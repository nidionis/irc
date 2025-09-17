/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_utils.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:21:18 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/09/17 11:11:17 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by n on 22/07/25.
//

#ifndef VECTOR_UTILS_TPP
# define VECTOR_UTILS_TPP

template <typename T>
bool	print_vec(std::vector<T>& v)
{
	typename std::vector<T>::iterator	it;

	for (it = v.begin(); it != v.end(); ++it)
	{ std::cout << "TEST_VECTOR : " << *it << "\n" ; }
	std::cout << std::endl;
	return (false);
};

template <typename T>
bool	is_in(std::vector<T>& v, const T x)
{
	typename std::vector<T>::iterator	it;

	for (it = v.begin(); it != v.end(); ++it)
		{ if (*it == x) { return (true); } }
	return (false);
};

template <typename T>
void	set(std::vector<T>& v, const T x)
{
	v.push_back(x);
	return ;
};

template <typename T>
void	del(std::vector<T>& v, T x)
{
	typename std::vector<T>::iterator	it;
	
	it = std::find(v.begin(), v.end(), x);
	if (it != v.end()) { v.erase(it); }
};

template <typename T>
bool	has(std::vector<T> v, T x)
{
	typename std::vector<T>::iterator	it;

	for (it = v.begin(); it != v.end(); ++it)
		{ if (*it == x) { return true; } }
	return (false);
};

#endif //VECTOR_UTILS_TPP
