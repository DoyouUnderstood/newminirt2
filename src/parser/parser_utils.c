/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erabbath <erabbath@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:41:43 by erabbath          #+#    #+#             */
/*   Updated: 2024/05/23 13:45:51 by erabbath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_parser.h"
#include "parser.h"

char	*parser_handle_error(void *p, char *msg)
{
	free(p);
	return (msg);
}

bool	parser_valid_intensity(double intensity)
{
	if (intensity < 0.0 || intensity > 1.0)
		return (false);
	return (true);
}

bool	parser_valid_color(int r, int g, int b)
{
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (false);
	return (true);
}

char	*parse_reflectivity(t_parser *parser, double *reflectivity)
{
	if (!parser_match(parser, " %f", reflectivity))
		*reflectivity = 0.0;
	return (NULL);
}

char	*parse_color(t_parser *parser, t_color *color)
{
	int	r;
	int	g;
	int	b;

	if (!parser_match(parser, "%d%_,%_%d%_,%_%d", &r, &g, &b))
		return ("Invalid color format");
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return ("Invalid color values");
	*color = color_from_rgb(r, g, b);
	return (NULL);
}

char	*parse_tuple(t_parser *parser, t_tuple *tuple,
	t_tuple (*tuple_creator_f)(double x, double y, double z))
{
	double	x;
	double	y;
	double	z;

	if (!parser_match(parser, "%f%_,%_%f%_,%_%f", &x, &y, &z))
		return ("Invalid tuple format");
	*tuple = tuple_creator_f(x, y, z);
	return (NULL);
}

char	*parse_pattern(t_parser *parser, t_pattern **pattern)
{
	t_parser_pattern	d;

	d.pattern_f = NULL;
	*pattern = NULL;
	if (parser_match(parser, " gradient "))
		d.pattern_f = gradient_pattern_create;
	else if (parser_match(parser, " checker "))
		d.pattern_f = checker_pattern_create;
	else if (parser_match(parser, " stripe "))
		d.pattern_f = stripe_pattern_create;
	else if (parser_match(parser, " ring "))
		d.pattern_f = ring_pattern_create;
	if (!d.pattern_f)
		return (NULL);
	if (!parser_match(parser, "%d,%d,%d %d,%d,%d",
		&d.r1, &d.g2, &d.b1, &d.r2, &d.g2, &d.b2))
		return ("Invalid pattern format");
	if (!parser_valid_color(d.r1, d.g1, d.b1)
			|| !parser_valid_color(d.r2, d.g2, d.b2))
		return ("Invalid pattern color");
	*pattern = d.pattern_f(color_from_rgb(d.r1, d.g1, d.b1),
			color_from_rgb(d.r2, d.g2, d.b2));
	if (!*pattern)
		return ("Pattern: malloc error");
	return (NULL);
}