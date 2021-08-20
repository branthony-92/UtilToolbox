#ifndef LISTENER_H
#define LISTENER_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include "MdlResponseInfo.h"
#include "RESTEndpoint.h"

namespace beast = boost::beast;             // from <boost/beast.hpp>
namespace http = beast::http;               // from <boost/beast/http.hpp>
namespace net = boost::asio;                // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;           // from <boost/asio/ssl.hpp>
using     tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class RESTServerContext;
typedef std::shared_ptr<RESTServerContext> TRESTCtxPtr;
typedef std::shared_ptr<ssl::context>    SSLCtxPtr;
typedef std::shared_ptr<net::io_context> IOCtxPtr;

/*  The class Session Handles an HTTP server connection
* 
    Extend from this class to define your session specific logic which will be called via the run() function.
    This class will be used by the Listener class as a prototype so the implimentation classes must override the
    clone() method to return a shared pointer to that subclass. 
 */
class SessionBase
{
protected:
    std::shared_ptr<beast::ssl_stream<beast::tcp_stream>> m_pStream;
    const std::string c_sessionName;
    std::atomic_bool m_closed;
    TRESTCtxPtr      m_pServerCtx;

public:

    explicit SessionBase(TRESTCtxPtr pServerCtx)
        : m_pStream(nullptr)
        , m_closed(true)
        , m_pServerCtx(pServerCtx)
    {}

    virtual std::shared_ptr<SessionBase> clone(tcp::socket&& sock, ssl::context& SSLCtx) = 0; // clone must move a socket 

    virtual void run() = 0;

    std::string getName() const { return c_sessionName; }
    bool isClosed() const { return m_closed.load(); };

    SplitQueries getQueries(beast::string_view target)
    {
        // find '?' to identify a key and '=' to identify values, all deliminated by '&'
        // find first query key
        SplitQueries queries{};
        
        size_t searchOffset = 0;
        size_t keyIndex = 0;
        size_t valueIndex = 0;
        size_t nextKey = 0;

        std::string str(target.data(), target.length());

        keyIndex = str.find_first_of('?', keyIndex);
        while (keyIndex != beast::string_view::npos && valueIndex != beast::string_view::npos && nextKey != beast::string_view::npos)
        {
            std::string key = "";
            std::string val = "";
            if (keyIndex == beast::string_view::npos) return queries;
            // look for the ampersand pos
            nextKey = str.find_first_of('&', keyIndex + 1);

            // look for a value for that key
            valueIndex = str.find_first_of('=', keyIndex);

            // there is no value for this key but maybe one for a future key
            if (valueIndex >= nextKey)
            {
                key = str.substr(keyIndex + 1, nextKey - keyIndex - 1).data();
            }
            else
            {
                key = str.substr(keyIndex + 1, valueIndex - keyIndex - 1).data();
                val = str.substr(valueIndex + 1, nextKey - valueIndex - 1).data();
            }

            keyIndex = nextKey;
            queries.insert_or_assign(key, val);
        }
        return queries;
    }
    std::string getEndpoint(beast::string_view target)
    {
        target.length();
        std::string ep(target.data(), target.length());
        auto index = 0;// ep.find_last_of("/api/");
        auto queryIndex = ep.find_first_of('?', index);
        if (index < ep.length())
        {
            ep = ep.substr(index, queryIndex - index).data();
            return ep;
        }
        return "";
    }


    // true constructor called by clone
    // Take ownership of the socket
    explicit SessionBase(
        tcp::socket&& socket, 
        ssl::context& SSLCtx,
        TRESTCtxPtr pServerCtx)
        : m_pStream(std::make_shared<beast::ssl_stream<beast::tcp_stream>>(std::move(socket), SSLCtx))
        , m_closed(false)
        , m_pServerCtx(pServerCtx)
    {}
protected:
};
typedef std::shared_ptr<SessionBase> SessionPtr;

class GenericSession : public SessionBase, public std::enable_shared_from_this<GenericSession>
{
    http::request<http::string_body> m_request;
    beast::flat_buffer m_buffer;

    std::shared_ptr<void> m_pResponse;
public:
    GenericSession(TRESTCtxPtr pServerCtx)
        : SessionBase(pServerCtx)
    {}
    virtual std::shared_ptr<SessionBase> clone(tcp::socket&& sock, ssl::context& SSLCtx) override // clone must move a socket 
    {
        return std::make_shared<GenericSession>(std::move(sock), SSLCtx, m_pServerCtx);
    }

    virtual void run() override
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch(
            m_pStream->get_executor(),
            beast::bind_front_handler(
                &GenericSession::onRun,
                shared_from_this()));  
    }

    void onRun()
    {
        // Set the timeout.
        beast::get_lowest_layer(*m_pStream).expires_after(std::chrono::seconds(30));

        // Perform the SSL handshake
        m_pStream->async_handshake(
            ssl::stream_base::server,
            beast::bind_front_handler(
                &GenericSession::onHandshake,
                shared_from_this()));   
    }
    void onHandshake(beast::error_code ec)
    {
        if (ec) {
            auto what = ec.message();
        }

        doRead();
    }
    void doRead()
    {
        // Make the request empty before reading,
      // otherwise the operation behavior is undefined.
        m_request = {};

        // Set the timeout.
        beast::get_lowest_layer(*m_pStream).expires_after(std::chrono::seconds(30));

        // Read a request
        http::async_read(*m_pStream, m_buffer, m_request,
            beast::bind_front_handler(
                &GenericSession::onRead,
                shared_from_this()));
    }
    void onRead(beast::error_code ec, std::size_t bytesTransferred)
    {
        boost::ignore_unused(bytesTransferred);

        // This means they closed the connection
        if (ec == http::error::end_of_stream)
            return doClose();

        if (ec)
            return doClose();
       
        // Send the response
        handleRequest(std::move(m_request));
    }

    
    template< class Body, class Allocator>
    std::string parseBody(http::request<Body, http::basic_fields<Allocator>> req)
    {
        return "";
    }
    template<class Allocator>
    std::string parseBody(http::request<http::string_body , http::basic_fields<Allocator >> req)
    {
        std::ostringstream oss;
        oss << req;
        return oss.str();
    }
    template< class Body, class Allocator>
    void handleRequest(http::request<Body, http::basic_fields<Allocator>>&& req);
    void onWrite(bool close, beast::error_code ec, std::size_t bytesTransferred)
    {
        boost::ignore_unused(bytesTransferred);
        return doClose();

        if (ec)
            return;

        if (close)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
        }

        // We're done with the response so delete it
        m_pResponse = nullptr;

        // Read another request
        doRead();
    }
    void doClose()
    {
        // Set the timeout.
        beast::get_lowest_layer(*m_pStream).expires_after(std::chrono::seconds(30));

        // Perform the SSL shutdown
        m_pStream->async_shutdown(
            beast::bind_front_handler(
                &GenericSession::onShutdown,
                shared_from_this()));
    }
    void onShutdown(beast::error_code ec)
    {
        if (ec)
            return;

            // At this point the connection is closed gracefully
    }

    explicit GenericSession(tcp::socket&& socket, ssl::context& SSLCtx, TRESTCtxPtr pServerCtx)
        : SessionBase(std::move(socket), SSLCtx, pServerCtx)
    {}
    protected:
};


/* 
    Accepts incoming connections and launches the sessions 
    The listening routine is pretty generic so we only really 
    need to pass it a session prototype to customize session behaviour
*/
class Listener : public std::enable_shared_from_this<Listener>
{
    net::io_context& m_IOContext;
    ssl::context&    m_SSLContext;
    tcp::acceptor    m_acceptor;
    SessionPtr       m_pSessionPrototype;
    std::vector<SessionPtr> m_liveSessions;

public:
    Listener(
        net::io_context& ioc,
        ssl::context& ctx,
        tcp::endpoint endpoint,
        SessionPtr pSessionPrototype);
    virtual ~Listener();

    // Start accepting incoming connections
    void run() { do_accept(); }

private:
    void do_accept();

    void on_accept(beast::error_code ec, tcp::socket socket);

    // Report a failure
    void  fail(beast::error_code ec, char const* what);

    void handleClosedSessions();
};
typedef std::shared_ptr<Listener> ListenerPtr;

template< class Body, class Allocator>
void GenericSession::handleRequest(http::request<Body, http::basic_fields<Allocator>>&& req)
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
        http::write(*m_pStream, res);
    };

    auto const method_error = [&](beast::string_view what)
    {
        http::response<http::string_body> res{ http::status::method_not_allowed, req.version() };
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "Method not supported" + std::string(what) + "'";
        res.prepare_payload();
        http::write(*m_pStream, res);
    };

    auto const server_error = [&](beast::string_view what)
    {
        http::response<http::string_body> res{ http::status::internal_server_error, req.version() };
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        http::write(*m_pStream, res);
    };

    if (!m_pServerCtx->hasHandlers())
    {
        throw std::exception("No Handlers registerd");
    }
    std::string body;
    std::shared_ptr<JSONInfoBody> pResponse = nullptr;
    try
    {
        beast::string_view target = req.target();
        ReqHandlerPtr pHandler = nullptr;
        // find the endpoint
        auto epString = getEndpoint(target);

        if (epString.find("/api/") != std::string::npos)
        {
            pHandler = m_pServerCtx->retrieveHandler(getEndpoint(target));
            body = parseBody(req);
        }
        else
        {
            pHandler = std::make_shared<WebPageRequestHandler>();
            body = epString;
        }

        if (!pHandler) throw std::exception("Failed to retrieve request handler");

        // extract the queries
        SplitQueries queries = getQueries(target);

        // extract the body

        beast::string_view methodStr = req.method_string();

        std::cout << "Request received: " << methodStr << "\n";

        // find the method
        http::verb method = req.method();
        switch (method)
        {
        case boost::beast::http::verb::delete_:
            pResponse = pHandler->handleRequest_Delete(queries, body, m_pServerCtx);
            break;
        case boost::beast::http::verb::get:
            pResponse = pHandler->handleRequest_Get(queries, body, m_pServerCtx);
            break;
        case boost::beast::http::verb::head:
            pResponse = pHandler->handleRequest_Head(queries, body, m_pServerCtx);
            break;
        case boost::beast::http::verb::post:
            pResponse = pHandler->handleRequest_Post(queries, body, m_pServerCtx);
            break;
        case boost::beast::http::verb::put:
            pResponse = pHandler->handleRequest_Put(queries, body, m_pServerCtx);
            break;
        default:
            break;
        }
        if (!pResponse) throw std::exception("Failed to retrieve response");

        switch (pHandler->getType())
        {
            case HTTPRequestHandler::DataType::JSON:
            {
                // dump the json body
                auto pResp = std::make_shared<http::response<http::string_body>>(http::status::ok, req.version());
                m_pResponse = pResp;

                pResp->set(http::field::server, BOOST_BEAST_VERSION_STRING);
                pResp->set(http::field::content_type, "text/html");
                pResp->keep_alive(req.keep_alive());
                pResp->body() = pResponse->toJSON().dump();
                pResp->prepare_payload();

                // Write the response
                http::async_write(
                    *m_pStream,
                    *pResp,
                    beast::bind_front_handler(
                        &GenericSession::onWrite,
                        shared_from_this(),
                        pResp->need_eof()));
                break;
            }
            case HTTPRequestHandler::DataType::WebPage:
            {
                // fetch the desired webpage
                // Respond to GET request
                beast::error_code ec;
                http::file_body::value_type body;

                auto j = std::dynamic_pointer_cast<ResponseInfoBody>(pResponse)->findBody("Web_Page_Info")->toJSON();
                std::string path = j["Path"].get<std::string>();

                if (path.empty())
                {
                    not_found(epString);
                    return;
                }

                body.open(path.c_str(), beast::file_mode::scan, ec);

                auto pResp = std::make_shared<http::response<http::file_body>>(
                    std::piecewise_construct,
                    std::make_tuple(std::move(body)),
                    std::make_tuple(http::status::ok, req.version()) );
                pResp->set(http::field::server, BOOST_BEAST_VERSION_STRING);
                pResp->set(http::field::content_type, "text/html");
                pResp->content_length(body.size());
                pResp->keep_alive(req.keep_alive());
                m_pResponse = pResp;
                // Write the response
                http::async_write(
                    *m_pStream,
                    *pResp,
                    beast::bind_front_handler(
                        &GenericSession::onWrite,
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

#endif // !LISTENER_H