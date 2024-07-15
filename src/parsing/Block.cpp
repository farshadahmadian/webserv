/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 11:55:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/15 12:00:59 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Block.hpp"

Block::Block( void ) {};
Block::~Block( void ) {};

std::vector<Block>& Block::getNestedBlock( void ) { return (_nested); };
std::map<std::string, std::string> Block::getSettings( void ) { return (_settings); };

void addNestedBlock( enum block type ) {

}
// void Block::setDefaults( void );
void Block::setSettings( std::string directive, std::string value/* , nesting_level */ ) {

}

// void checkConfiguration( void ); // ------------------------ CONFIGURATION

// void displaySettings( void ); // -------------------------- DISPLAY

// static const std::string sharedDirectives[5];

// Block( const Block& ); // --------------------------- UNUSED CONSTRUCTORS
// void operator=( const Block& );
