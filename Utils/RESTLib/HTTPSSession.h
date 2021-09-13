#ifndef HTTPS_SESSION_H
#define HTTPS_SESSION_H

#include "Session.h"

class GenericSessionHTTPS 
    : public SessionBase
    , public std::enable_shared_from_this<GenericSessionHTTPS>
{
    std::shared_ptr<beast::ssl_stream<beast::tcp_stream>> m_pHTTPS_Stream;
    http::request<http::string_body> m_request;
    beast::flat_buffer m_buffer;

    std::shared_ptr<void> m_pResponse;
public:
    GenericSessionHTTPS(TRESTCtxPtr pServerCtx)
        : SessionBase(pServerCtx, true)
        , m_pHTTPS_Stream(nullptr)
    {}
    virtual std::shared_ptr<SessionBase> clone(tcp::socket&& sock, ssl::context& SSLCtx) override // clone must move a socket 
    {
        auto pSession = std::make_shared<GenericSessionHTTPS>(std::move(sock), SSLCtx, m_pServerCtx);
        return pSession;
    }

    virtual void run() override;

    void onRun();
    void onHandshake(beast::error_code ec);

    void doRead();
    void onRead(beast::error_code ec, std::size_t bytesTransferred);


    template< class Body, class Allocator>
    std::string extractBody(http::request<Body, http::basic_fields<Allocator>> req)
    {
        return "";
    }
    template<class Allocator>
    std::string extractBody(http::request<http::string_body, http::basic_fields<Allocator >> req)
    {
        std::ostringstream oss;
        oss << req;
        return oss.str();
    }

    template< class Body, class Allocator>
    void handleRequest(http::request<Body, http::basic_fields<Allocator>>&& req);

    void onWrite(bool close, beast::error_code ec, std::size_t bytesTransferred);
    void doClose();
    void onShutdown(beast::error_code ec);

    explicit GenericSessionHTTPS(tcp::socket&& socket, ssl::context& SSLCtx, TRESTCtxPtr pServerCtx)
        : SessionBase(pServerCtx, true)
        , m_pHTTPS_Stream(std::make_shared<beast::ssl_stream<beast::tcp_stream>>(std::move(socket), SSLCtx))
    {}

protected:
    beast::string_view  mime_type(beast::string_view path);
};


template< class Body, class Allocator>
void GenericSessionHTTPS::handleRequest(http::request<Body, http::basic_fields<Allocator>>&& req)
{
    // Returns a server error response
    // Returns a not found response
    auto const not_found = [&](beast::string_view target)
    {
        http::response<http::string_body> res{ http::status::not_found, req.version() };
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        http::write(*m_pHTTPS_Stream, res);
    };

    auto const method_error = [&](beast::string_view what)
    {
        http::response<http::string_body> res{ http::status::method_not_allowed, req.version() };
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "Method not supported" + std::string(what) + "'";
        res.prepare_payload();
        http::write(*m_pHTTPS_Stream, res);
    };

    auto const server_error = [&](beast::string_view what)
    {
        http::response<http::string_body> res{ http::status::internal_server_error, req.version() };
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        http::write(*m_pHTTPS_Stream, res);
    };

    auto transID = m_pServerCtx->getNextransactionID();

    if (!m_pServerCtx->hasHandlers())
    {
        throw std::exception("No Handlers registerd");
    }
    std::shared_ptr<JSONInfoBody> pResponse = nullptr;
    try
    {
        beast::string_view target = req.target();
        ReqHandlerPtr pHandler = nullptr;
        // find the endpoint
        auto epString = extractEndpoint(target);

        if (epString.find("/api/") != std::string::npos)
        {
            pHandler = m_pServerCtx->retrieveHandler(epString);
        }
        else
        {
            pHandler = std::make_shared<WebPageRequestHandler>();
        }
        if (!pHandler) throw std::exception("Failed to retrieve request handler");

        pHandler->resetResponseBody();

        // retrieve the JSON body from the request body if there is one
        std::string body = extractBody(req);
        auto jsonStart = body.find_first_of('{');
        auto jsonEnd = body.find_last_of('}');

        if (jsonStart != std::string::npos && jsonEnd != std::string::npos)
        {
            body = body.substr(jsonStart, jsonEnd - jsonStart + 1);
        }
        else
        {
            body = "";
        }


        // extract the queries
        QueryList queries(extractQueries(target));

        // extract the body

        beast::string_view methodStr = req.method_string();

        std::cout << "Request received: " << methodStr << "\n";

        // find the method and handle the request
        http::verb method = req.method();
        switch (method)
        {
        case boost::beast::http::verb::delete_:
            pResponse = pHandler->handleRequest_Delete(epString, queries, body, m_pServerCtx);
            break;
        case boost::beast::http::verb::get:
            pResponse = pHandler->handleRequest_Get(epString, queries, body, m_pServerCtx);
            break;
        case boost::beast::http::verb::head:
            pResponse = pHandler->handleRequest_Head(epString, queries, body, m_pServerCtx);
            break;
        case boost::beast::http::verb::post:
            pResponse = pHandler->handleRequest_Post(epString, queries, body, m_pServerCtx);
            break;
        case boost::beast::http::verb::put:
            pResponse = pHandler->handleRequest_Put(epString, queries, body, m_pServerCtx);
            break;
        default:
            break;
        }
        if (!pResponse) throw std::exception("Failed to retrieve response");

        // get the handler type and process the response
        switch (pHandler->getType())
        {
        case HTTPRequestHandler::DataType::JSON:
        {
            // dump the json body
            auto pResp = std::make_shared<http::response<http::string_body>>(http::status::ok, req.version());
            pResp->set(http::field::server, BOOST_BEAST_VERSION_STRING);
            pResp->set(http::field::content_type, "text/html");
            pResp->keep_alive(req.keep_alive());
            pResp->body() = pResponse->toJSON().dump();
            pResp->prepare_payload();
            m_pResponse = pResp;

            // Write the response
            http::async_write(*m_pHTTPS_Stream, *pResp,
                beast::bind_front_handler(
                    &GenericSessionHTTPS::onWrite,
                    shared_from_this(),
                    pResp->need_eof()
                )
            );
            break;
        }
        case HTTPRequestHandler::DataType::WebPage:
        {
            // fetch the desired webpage
            // Respond to GET request
            beast::error_code ec;
            http::file_body::value_type body;

            auto pWebPageBody = std::dynamic_pointer_cast<WebPageInfoBody>(pResponse);
            if (!pWebPageBody) throw std::exception("Incorrect response body type");

            std::string path = pWebPageBody->getPath();

            if (path.empty())
            {
                not_found(path);
                return;
            }

            std::cout << "Resource requested: " << path << "\n";

            body.open(path.c_str(), beast::file_mode::scan, ec);
            if (ec)
            {
                not_found(path);
                return;
            }
            const auto type = mime_type(path);
            const auto size = body.size();
            auto pResp = std::make_shared<http::response<http::file_body>>(
                std::piecewise_construct,
                std::make_tuple(std::move(body)),
                std::make_tuple(http::status::ok, req.version()));
            pResp->set(http::field::server, BOOST_BEAST_VERSION_STRING);
            pResp->set(http::field::content_type, type);
            pResp->content_length(size);
            pResp->keep_alive(req.keep_alive());
            m_pResponse = pResp;

            // Write the response
            http::async_write(*m_pHTTPS_Stream, *pResp,
                beast::bind_front_handler(
                    &GenericSessionHTTPS::onWrite,
                    shared_from_this(),
                    pResp->need_eof()));
            break;
        }
        default:
            server_error("Unknown handler type");
            break;
        }
    }
    catch (std::exception& err)
    {
        server_error(err.what());
    }
}


#endif  