#include "stdafx.h"
#include "RESTServer.h"
#include "PasswordGenerator.h"
#include "PasswordGeneratorEP.h"
#include "RESTModels.h"

const std::string c_configFilePath = "C:\\Users\\xs_br\\source\\repos\\branthony-92\\UtilToolbox\\Utils\\PasswordGenerator\\Config.json";

int main(int argc, char* argv[])
{
	auto pPasswordGen = std::make_shared<PasswordGenerator>();
	auto pServer = std::make_shared<Server>(pPasswordGen, 8);

	auto pInfo = pServer->getServerInfo();
	pInfo->setServerName("Brian Test Server");
	pInfo->setAPIVer(1.0);
	pInfo->setIdleTimeout(300u); // 5 min timeout
	pPasswordGen->setServerInfo(pInfo);

	auto pHandler = std::make_shared<PasswordGeneratorEP>();

	pPasswordGen->registerHandler(pHandler->getName(), pHandler);

	auto ssl_callback = [](ssl::context& ctx) {
		try {
			printf("SSL Context Callback entered\n");
			ctx.set_options(
				ssl::context::default_workarounds |
				ssl::context::no_sslv2 |
				ssl::context::no_tlsv1 |
				ssl::context::no_tlsv1_1 |
				ssl::context::single_dh_use
			);

			ctx.set_password_callback(
				[](std::size_t /*max_length*/, ssl::context::password_purpose /*purpose*/)
			{ return "test"; }
			);
			beast::error_code ec;

			nlohmann::json config = jsonUtils::loadFile(c_configFilePath);
			if (config.empty())
			{
				throw std::runtime_error("Configuration file not found");
			}

			nlohmann::json serverConfig = jsonUtils::extractValue<nlohmann::json>(config, "Server_Config", nlohmann::json::object());
			if (serverConfig.empty())
			{
				throw std::runtime_error("Server configuration not found");
			}

			std::string certPath = jsonUtils::extractValue<std::string>(serverConfig, "Certificate_Path", "");
			if (certPath.empty()) throw std::runtime_error("Path to certificate not found");

			std::string keyPath = jsonUtils::extractValue<std::string>(serverConfig, "Priv_Key_Path", "");
			if (keyPath.empty()) throw std::runtime_error("Path to private key not found");

			std::string dhParamPath = jsonUtils::extractValue<std::string>(serverConfig, "DH_Params_Path", "");
			if (dhParamPath.empty()) throw std::runtime_error("Path to Diffie-Hellman parameters not found");

			ctx.use_certificate_chain_file(certPath);
			ctx.use_private_key_file(keyPath, ssl::context::pem);
			ctx.use_tmp_dh_file(dhParamPath);
		}
		catch (std::exception const& e) {
			std::clog << "ERROR: " << e.what() << "\n";
		}
	};

	auto pSessionPrototyp = std::make_shared<GenericSession>(pPasswordGen);
	pServer->startServer("192.168.2.16", 8080, pSessionPrototyp, ssl_callback);
	while (true);
}