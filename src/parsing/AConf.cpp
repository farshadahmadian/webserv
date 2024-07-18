/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConf.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:24:02 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/18 13:59:57 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AConf.hpp"

AConf::AConf( std::map<std::string, std::string> settings, context type ): _type(type), _settings(settings) {
	setSharedSettingsDefaults();
};
AConf::AConf( context type ): _type(type) {
	setSharedSettingsDefaults();
};
const std::map<std::string, std::string> AConf::getSettings( void ) const { return (_settings); };

const std::string AConf::sharedSettings[N_SHARED_DIR] = {
	"keepalive_timeout",
	"client_body_buffer_size",
	"root",
	"index",
	"autoindex"
};

void AConf::setSharedSettingsDefaults( void ) {
	_settings["keepalive_timeout"] = DEFAULT_TIMEOUT;
	_settings["client_body_buffer_size"] = DEFAULT_CLIENT_SIZE;
	_settings["index"] = DEFAULT_INDEX;
	_settings["autoindex"] = DEFAULT_AUTOINDEX;
	_settings["root"] = DEFAULT_ROOT;
};

enum conf_err AConf::checkSharedSettings( void ) {
	// TIMEOUT			"75"				: set a limit?
	// CLIENT_SIZE		"80"				: set a limit?
	if (_settings["autoindex"] != "off" && _settings["autoindex"] != "on") return (E_AUTOINDEX);
	if (!isValidNumber("keepalive_timeout", _settings["keepalive_timeout"])) return (E_TIMEOUT);
	if (!isValidNumber("client_body_buffer_size", _settings["client_body_buffer_size"])) return (E_CLIENTSIZE);
	return (CONF_SUCCESS);
}
