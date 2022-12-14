/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgulenay <mgulenay@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 13:03:25 by mgulenay          #+#    #+#             */
/*   Updated: 2022/11/07 16:32:45 by mgulenay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/cub3d.h"

static int	cub3d_texture_assign(t_data *data);
static int	cub3d_texture_address_set(t_data *data);

// IF IT RETURN FAILURE MAP DOES NOT EXIST
int	cub3d_texture_init(t_data *data)
{
	if (cub3d_texture_assign(data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	cub3d_texture_address_set(data);
	return (EXIT_SUCCESS);
}

static int	cub3d_texture_assign(t_data *data)
{
	data->tex.no.img = mlx_xpm_file_to_image(data->mlx,
			data->tex.no.path, &data->tex_size, &data->tex_size);
	data->tex.so.img = mlx_xpm_file_to_image(data->mlx,
			data->tex.so.path, &data->tex_size, &data->tex_size);
	data->tex.ea.img = mlx_xpm_file_to_image(data->mlx,
			data->tex.ea.path, &data->tex_size, &data->tex_size);
	data->tex.we.img = mlx_xpm_file_to_image(data->mlx,
			data->tex.we.path, &data->tex_size, &data->tex_size);
	if (data->tex.no.img == NULL || data->tex.so.img == NULL
		|| data->tex.ea.img == NULL || data->tex.we.img == NULL)
	{
		data->error_check = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	cub3d_texture_address_set(t_data *data)
{
	data->tex.no.addr = (int *)mlx_get_data_addr(data->tex.no.img,
			&data->tex.no.bits_per_pixel, &data->tex.no.line_length,
			&data->tex.no.endian);
	data->tex.so.addr = (int *)mlx_get_data_addr(data->tex.so.img,
			&data->tex.so.bits_per_pixel, &data->tex.so.line_length,
			&data->tex.so.endian);
	data->tex.ea.addr = (int *)mlx_get_data_addr(data->tex.ea.img,
			&data->tex.ea.bits_per_pixel, &data->tex.ea.line_length,
			&data->tex.ea.endian);
	data->tex.we.addr = (int *)mlx_get_data_addr(data->tex.we.img,
			&data->tex.we.bits_per_pixel, &data->tex.we.line_length,
			&data->tex.we.endian);
	return (EXIT_SUCCESS);
}

void	cub3d_calculate_wallx(t_data *data)
{
	if (data->ray.side == 0)
		data->tex.wall_x = data->player.y
			+ data->ray.wall_dist * data->ray.pos.y;
	else
		data->tex.wall_x = data->player.x
			+ data->ray.wall_dist * data->ray.pos.x;
	data->tex.wall_x -= floor(data->tex.wall_x);
	data->tex.coord.x = (int)(data->tex.wall_x * (double) data->tex_size);
	if (data->ray.side == 0 && data->ray.pos.x > 0)
		data->tex.coord.x = data->tex_size - data->tex.coord.x - 1;
	if (data->ray.side == 1 && data->ray.pos.y < 0)
		data->tex.coord.x = data->tex_size - data->tex.coord.x - 1;
	data->tex.step = 1.0 * data->tex_size / data->draw.line_height;
	data->tex.tex_pos = (data->draw.start - HEIGHT
			/ 2 + data->draw.line_height / 2) * data->tex.step;
}

void	cub3d_get_texture(t_data *data)
{
	data->tex.coord.y = (int)data->tex.tex_pos & (data->tex_size - 1);
	data->tex.tex_pos += data->tex.step;
	if (data->ray.side == 1)
	{
		if (data->ray.pos.y > 0)
			data->tex.colour = data->tex.so.addr
			[data->tex_size * data->tex.coord.y + data->tex.coord.x];
		else
			data->tex.colour = data->tex.no.addr
			[data->tex_size * data->tex.coord.y + data->tex.coord.x];
	}
	else
	{
		if (data->ray.pos.x < 0)
			data->tex.colour = data->tex.ea.addr
			[data->tex_size * data->tex.coord.y + data->tex.coord.x];
		else
			data->tex.colour = data->tex.we.addr
			[data->tex_size * data->tex.coord.y + data->tex.coord.x];
	}
}
