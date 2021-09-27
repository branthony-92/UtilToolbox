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

			// These objects perform our I/O
			tcp::resolver resolver(ioc);
			beast::tcp_stream stream(ioc);

			// Look up the domain name
			auto const results = resolver.resolve(host, port);

			stream.connect(results);

			// Perform the SSL handshake
			beast::error_code ec;
		
			// Set up an HTTP GET request message

			http::request<http::string_body> req{ verb, target.c_str(), version };
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

			// Write the message to standard out
			std::cout << res << std::endl;

			// read the response in to the body
			pResponse->setResponse(res);
			
			// Gracefully close the stream
			stream.socket().shutdown(tcp::socket::shutdown_both, ec);
			// not_connected happens sometimes
			// so don't bother reporting it.
			//
			if (ec && ec != beast::errc::not_connected)
				throw beast::system_error{ ec };
	
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
