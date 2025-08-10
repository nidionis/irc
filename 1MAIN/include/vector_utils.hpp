/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:43:43 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/10 10:30:19 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by n on 22/07/25.
//

#ifndef VECTOR_UTILS_HPP
# define VECTOR_UTILS_HPP

# include <vector>

// Template implementation in header file
template <typename T>
bool	is_in(std::vector<T> &v, const T x);

template <typename T>
void	set(std::vector<T> &v, const T x);

// Corrected version of the template function
template <typename T>
void	del(std::vector<T>& v, T x);

template <typename T>
bool	has(std::vector<T> v, T x);

# include "../src/vector_utils.tpp"

#endif //VECTOR_UTILS_HPP
