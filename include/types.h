/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erabbath <erabbath@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:08:29 by erabbath          #+#    #+#             */
/*   Updated: 2024/05/15 16:08:30 by erabbath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

typedef struct s_tuple
{
	double	x;
	double	y;
	double	z;
	double	w;
}			t_tuple;
typedef struct s_color
{
	double	r;
	double	g;
	double	b;
}			t_color;

typedef struct s_matrix
{
	float	m[4][4];
}			t_matrix;

#endif
