/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_strings.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:43:41 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/10 10:36:20 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by n on 17/07/25.
//

#ifndef UTILS_STRINGS_HPP
# define UTILS_STRINGS_HPP

# include <algorithm>
# include <sstream>
# include <stdint.h> // uint16_t

# define TRIMMED_CHARS	": \t\r\n"
# define LEN_MAX_NAME	16

std::string	trim(const std::string& str);
std::string	trim(const std::string& str, const std::string& chars);
std::string	getHead(const std::string& str);
std::string	getNextWds(const std::string& str);
std::string	lowerCase(const std::string& str);
std::string	upperCase(const std::string& str);
bool		isValidName(const std::string& str);
std::string	to_string(const ssize_t value);
std::string	popWd(std::string& str);
std::string	lastWord(std::string& str);
std::string	ft_put_uint16(uint16_t num);

#endif //UTILS_STRINGS_HPP
