/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erabbath <erabbath@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 09:22:19 by erabbath          #+#    #+#             */
/*   Updated: 2024/05/23 07:43:47 by erabbath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_parser.h"
#include "ft_parser_internal.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

void	parser_init(t_parser *parser, char *s)
{
	parser->s = s;
	parser->pos = 0;
}

bool	parser_match(t_parser *parser, char *fmt, ...)
{
	va_list	args;
	bool	result;

	va_start(args, fmt);
	result = true;
	while (result && *fmt)
	{
		if (*fmt == '%')
			result = parser_match_handle_special(parser, &fmt, args);
		else if (*fmt == ' ')
			result = parser_match_handle_mandatory_space(parser, &fmt);
		else if (!parser_match_char(parser, *fmt++))
			return (false);
	}
	va_end(args);
	return (result);
}
