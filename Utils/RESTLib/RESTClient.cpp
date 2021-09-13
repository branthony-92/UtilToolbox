#include "StdAfx.h"
#include "RESTClient.h"


RequestPromisePtr Client::makeRequest(RequestPtr pReq, http::verb verb)
{
	ResponsePtr pResponse = std::make_shared<Response>();
	auto future = std::async(std::launch::async, [this, pReq, verb, pResponse]
	{
		auto const host   = pReq->m_address.c_str();
		auto const port   = pReq->m_port.c_str();
		auto target       = pReq->m_target;

		if (!pReq->m_queries.empty())
		{
			target.append("?");
			for (auto q = pReq->m_queries.begin(); q != pReq->m_queries.end();)
			{
				target.append(q->first);
				if (!q->second.empty())
				{
					target.append("=");
					target.append(q->second);
				}
				q++;
				if (q != pReq->m_queries.end())
				{
					target.append("&");
				}
			}
		}

		auto const version = 11;
		try
		{
			// The io_context is required for all I/O
			net::io_context ioc;

			// The SSL context is required, and holds certificates
			ssl::context ctx(ssl::context::tlsv12_client);

			m_fSSLInitHandler(ctx);

			// Verify the remote server's certificate
			ctx.set_verify_mode(ssl::verify_none);

			// These objects perform our I/O
			tcp::resolver resolver(ioc);
			beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

			// Set SNI Hostname (many hosts need this to handshake successfully)
			if (!SSL_set_tlsext_host_name(stream.native_handle(), host))
			{
				beast::error_code ec{ static_cast<int>(::ERR_get_error()), net::error::get_ssl_category() };
				throw beast::system_error{ ec };
			}

			// Look up the domain name
			auto const results = resolver.resolve(host, port);

			// Make the connection on the IP address we get from a lookup
			beast::get_lowest_layer(stream).connect(results);

			// Perform the SSL handshake
			beast::error_code ec;
			stream.handshake(ssl::stream_base::client, ec);
			if (ec)
			{
				auto why = ec.message();
				throw std::exception(why.c_str());
			}
			// Set up an HTTP GET request message

			http::request<http::string_body> req{ verb, target.c_str(), 10 };
			req.set(http::field::host, host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

			// Send the HTTP request to the remote host
			http::write(stream, req, ec);
			if (ec)
			{
				auto why = ec.message();
				throw std::exception(why.c_str());
			}
			pReq->m_status = RequestStatus::RequestSent;

			// This buffer is used for reading and must be persisted
			beast::flat_buffer buffer;

			// Declare a container to hold the response
			http::response<http::dynamic_body> res;

			// Receive the HTTP response
			http::read(stream, buffer, res, ec);
			if (ec)
			{
				auto why = ec.message();
				throw std::exception(why.c_str());
			}

			// read the response in to the body
			pResponse->setResponse(res);
			
			// Write the message to standard out
			std::cout << res << std::endl;

			// Gracefully close the stream
			stream.shutdown(ec);
			if (ec == net::error::eof)
			{
				// Rationale:
				// http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
				ec = {};
			}
			if (ec) throw beast::system_error{ ec };

			// If we get here then the connection is closed gracefully
			pReq->m_status = RequestStatus::ResponseReady;
			return pResponse;
		}
		catch (std::exception const& e)
		{
			pReq->m_status = RequestStatus::Error;
			pResponse->setError(e.what());
			return pResponse;
		}
	});
	auto pPromise = std::make_shared<RequestPromise>(pReq->m_status, future.share());
	return pPromise;
}
