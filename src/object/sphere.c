/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erabbath <erabbath@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 17:53:22 by erabbath          #+#    #+#             */
/*   Updated: 2024/05/23 09:43:21 by erabbath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object.h"
#include "sphere.h"

#include <float.h>
#include <math.h>
#include <stdlib.h>

static t_sphere_data	*sphere_create_data(t_tuple center, double radius)
{
	t_sphere_data	*sphere_data;

	sphere_data = malloc(sizeof(t_sphere_data));
	if (!sphere_data)
		return (NULL);
	sphere_data->center = center;
	sphere_data->radius = radius;
	return (sphere_data);
}

static t_intersection	*create_intersections(t_object *obj, double t1, double t2, double discriminant,
		int *out_count)
{
	t_intersection	*intersections;

	if (discriminant == 0)
	{
		*out_count = 1;
		intersections = intersection_create(obj, t1);
	}
	else
	{
		*out_count = 2;
		intersections = intersection_create_pair(obj, t1, t2);
	}
	if (!intersections)
	{
		*out_count = 0;
		return (NULL);
	}
	return (intersections);
}

static t_intersection	*sphere_intersect(t_object *obj, t_ray *ray,
		int *out_count)
{
	t_sphere_data			*sphere;
	t_sphere_intersect_calc	calc;

	sphere = (t_sphere_data *) obj->obj;
	calc.sphere_to_ray = tuple_subtract(ray->origin, sphere->center);
	calc.a = vector_dot(ray->direction, ray->direction);
	calc.b = 2 * vector_dot(ray->direction, calc.sphere_to_ray);
	calc.c = vector_dot(calc.sphere_to_ray, calc.sphere_to_ray) - 1.0;
	calc.discriminant = calc.b * calc.b - 4 * calc.a * calc.c;
	if (calc.discriminant < -EPSILON)
	{
		*out_count = 0;
		return (NULL);
	}
	calc.discriminant_sqrt = sqrt(calc.discriminant);
	return (create_intersections(obj,
		(-calc.b - calc.discriminant_sqrt) / (2 * calc.a),
		(-calc.b + calc.discriminant_sqrt) / (2 * calc.a),
		calc.discriminant,
		out_count));
}

static t_tuple	sphere_normal_at(t_shape *shape, t_tuple local_point)
{
	t_tuple	tupl;

	(void)shape;
	tupl = tuple_subtract(local_point, point_create(0, 0, 0));
	return (tupl);
}

t_object	*sphere_create(t_tuple center, double radius,
		t_material_specs specs)
{
	t_sphere_data	*sphere_data;
	t_object		*obj;

	sphere_data = sphere_create_data(center, radius);
	obj = malloc(sizeof(t_object));
	obj->type = SPHERE;
	obj->obj = sphere_data;
	obj->shape = malloc(sizeof(t_shape));
	obj->shape->transformation = matrix_mult(matrix_translation(center.x,
				center.y, center.z), matrix_scaling(radius, radius, radius));
	obj->shape->inv_transformation = matrix_inverse(obj->shape->transformation);
	obj->shape->tinv_transformation = matrix_transpose(obj->shape->inv_transformation);
	obj->shape->material = material_create_default(&specs.color,
			specs.reflectivity, specs.pattern);
	obj->shape->local_intersect = sphere_intersect;
	obj->shape->local_normal_at = sphere_normal_at;
	sphere_data->center = (t_tuple){0, 0, 0, 0};
	return (obj);
}
