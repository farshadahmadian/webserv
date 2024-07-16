/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConf.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:24:02 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/16 15:32:44 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AConf.hpp"

AConf::AConf( std::map<std::string, std::string> settings, context type ): _type(type), _settings(settings) {
	setSharedSettingsDefaults();
};
AConf::AConf( context type ): _type(type) {
	setSharedSettingsDefaults();
};
std::map<std::string, std::string> AConf::getSettings( void ) { return (_settings); };

const std::string AConf::sharedDirectives[N_SHARED_DIR] = {
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
