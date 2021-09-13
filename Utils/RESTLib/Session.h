#ifndef SESSION_H
#define SESSION_H

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

    std::atomic_bool  m_closed;
    TRESTCtxPtr       m_pServerCtx;

public:
    const std::string c_sessionName;
    const bool        m_useSSLTLS;

    explicit SessionBase(TRESTCtxPtr pServerCtx, bool useSSL = false)
        : m_closed(true)
        , m_useSSLTLS(useSSL)
        , m_pServerCtx(pServerCtx)
    {}

    // clone must move a socket, the SSL context is only needed for HTTPS sessions
    virtual std::shared_ptr<SessionBase> clone(tcp::socket&& sock, ssl::context& SSLCtx) = 0; 

    virtual void run() = 0;

    std::string getName() const { return c_sessionName; }
    bool isClosed() const { return m_closed.load(); };

    void setClosed(bool closed) { m_closed = closed; }

    SplitQueries extractQueries(beast::string_view target);
    std::string extractEndpoint(beast::string_view target);

protected:
};
typedef std::shared_ptr<SessionBase> SessionPtr;

#endif // !SESSION_H

