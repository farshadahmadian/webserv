/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 11:47:17 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/15 12:03:14 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <map>

enum block {
	HTTP,
	SERVER,
	LOCATION
};

class Block
{
	public: // ---------------------------------------------- USED CONSTRUCTORS
		Block( void );
		~Block( void );

		std::vector<Block>& getNestedBlock( void );
		std::map<std::string, std::string> getSettings( void );

		void addNestedBlock( enum block type );
		void setDefaults( void );
		void setSettings( std::string directive, std::string value/* , nesting_level */ );

		void checkConfiguration( void ); // ------------------------ CONFIGURATION

		void displaySettings( void ); // -------------------------- DISPLAY

	protected:
		static const std::string sharedDirectives[5];

	private: // ----------------------------------------------- PRIVATE MEMBERS
		block _type;
		std::vector<Block> _nested;
		std::map<std::string, std::string> _settings;

		Block( const Block& ); // --------------------------- UNUSED CONSTRUCTORS
		void operator=( const Block& );
};
