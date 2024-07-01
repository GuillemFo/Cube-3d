/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wil <wil@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:04:16 by gforns-s          #+#    #+#             */
/*   Updated: 2024/07/01 18:55:23 by wil              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void    wall_v_hit(t_graph *g, t_ray *r)
{
	r->wvhx = r->fvhx;
	r->wvhy = r->fvhy;
	r->deltay = BLOCK_SIZE * r->diry / fabs(r->dirx);
	r->wvhl = 0;
	while (!r->wvhl)
	{
		if (r->wvhy < 0 || r->wvhy >= g->file->max_y * BLOCK_SIZE)
			r->wvhl = -1;
		else if (get_map_char(g, r->wvhx - (r->dirx < 0), r->wvhy) == '1')
		{
			r->wvhl = fabs((r->wvhx - g->p.povx) / r->dirx);
			if (!r->wvhl)
				r->wvhl = 0.0000000000001;
		}
		else
		{
			r->wvhx += BLOCK_SIZE * (r->dirx > 0) - BLOCK_SIZE * (r->dirx < 0);
			r->wvhy += r->deltay;
		}
	}
	r->wvhl *= fabs(cos(r->raya - g->p.pova));
	// printf("last Vertical hit X : %.2f Y : %.2f L: %.2f\n", r->wvhx, r->wvhy, r->wvhl);
}

void	wall_h_hit(t_graph *g, t_ray *r)
{
	r->whhy = r->fhhy;
	r->whhx = r->fhhx;
	r->whhl = 0;
	r->deltax = BLOCK_SIZE * r->dirx / fabs(r->diry);
	while (!r->whhl)
	{
		if (r->whhx < 0 || r->whhx >= g->file->max_x * BLOCK_SIZE)
			r->whhl = -1;
		else if (get_map_char(g, r->whhx, r->whhy - (r->diry < 0)) == '1')
		{
			r->whhl = fabs((r->whhy - g->p.povy) / r->diry);
			if (!r->whhl)
				r->whhl = 0.0000000000001;
		}
		else
		{
			r->whhy += BLOCK_SIZE * (r->diry > 0) - BLOCK_SIZE * (r->diry < 0);
			r->whhx += r->deltax;
		}
	}
	r->whhl *= fabs(cos(r->raya - g->p.pova));
	// printf("last Horizontal hit X : %.2f Y : %.2f L: %.2f\n", r->whhx, r->whhy, r->whhl);
}
void    get_first_hit(t_ray *r)	
{
	r->dirx = cos(r->raya);
	r->diry = -sin(r->raya);
	r->fvhx = (i_coor(r->pos_x) + (r->dirx > 0)) * BLOCK_SIZE;
	if (r->dirx == 0)
		r->fvhy = -1;
	else
		r->fvhy = (r->fvhx - r->pos_x) * r->diry / r->dirx + r->pos_y;
	r->fhhy = (i_coor(r->pos_y) + (r->diry > 0)) * BLOCK_SIZE;
	if (r->diry == 0)
		r->fhhx = -1;
	else
		r->fhhx = (r->fhhy - r->pos_y) * r->dirx / r->diry + r->pos_x;
/*	printf("Pos X : %.2f Y : %.2f\n", r->pos_x, r->pos_y);
	printf("Vertical hit X : %.2f Y : %.2f\n", r->fvhx, r->fvhy);
	printf("Horizontal hit X : %.2f Y : %.2f\n", r->fhhx, r->fhhy);
*/
}

int	ray_inside(t_file *f, double x, double y)
{
	int grid_x;
	int grid_y;

	grid_x = (int)(x / BLOCK_SIZE); //i_coor
	grid_y = (int)(y / BLOCK_SIZE);

printf("Y=%d-- X=%d--\n", f->max_y, f->max_x);
	if (grid_y >= 0 && grid_y < f->max_y - 1)
	{
		ft_printf("1\n");
		if (grid_x >= 0 && grid_x < f->max_x - 1) //added -1 to compensate for array starting at 0.
		{
			ft_printf("2\n");
			ft_printf("is %c\n", f->map[ 2 + grid_y][ 2 + grid_x]);
			if (f->map[2 + grid_y][2 + grid_x] != '1' && f->map[2 + grid_y][2 + grid_x] != ' ') ////the +2 is due extra space around the map to not read out of memory
				return (1);
		}
		ft_printf("3\n");
	}
	return (0);
}


void    loop_rays(t_graph *g)
{
    int i;

    g->ray.pos_x = g->p.povx;
    g->ray.pos_y = g->p.povy;
    g->ray.raya = g->p.pova + FIELD_OF_VIEW / 2;
    if (g->ray.raya > 2 * M_PI)
        g->ray.raya -= 2 * M_PI;
    i = 0;
    //if (i < WIN_X)	
	while (i < WIN_X)
    {
        get_first_hit(&g->ray);
		wall_v_hit(g, &g->ray);
		wall_h_hit(g, &g->ray);
		if (g->ray.wvhl < 0 
			|| (g->ray.whhl >=0 && g->ray.whhl <= g->ray.wvhl))
		{
			// take whhl to calculate sow, sidex, side, ...
			g->ray.soi = (g->ray.diry <= 0);
			g->ray.ooi = fmod(g->ray.whhx, BLOCK_SIZE);
			if (g->ray.soi == 0)
				g->ray.ooi = BLOCK_SIZE - g->ray.ooi;
			g->ray.sow = g->p.bs * g->p.ppd / g->ray.whhl;
		}
		else // there is hit for sure
		{
			// take wvhl to calculate sow, sidex, side, ...
			g->ray.soi = 2 + (g->ray.dirx > 0);
			g->ray.ooi = fmod(g->ray.wvhy, BLOCK_SIZE);
			if (g->ray.soi == 2)
				g->ray.ooi = BLOCK_SIZE - g->ray.ooi;
			g->ray.sow = g->p.bs * g->p.ppd / g->ray.wvhl;
		}
		if (g->ray.sow > WIN_Y)
			g->ray.sow = WIN_Y;
		//printf("Side %d, Offset: %.2f, SOW: %.2f\n", g->ray.soi, g->ray.ooi, g->ray.sow);
		draw_column(g, i, (int)g->ray.sow, 0);
		draw_texture(g, i++, (int)g->ray.sow, g->ray.soi, g->ray.ooi, 0);
		mlx_put_image_to_window(g->mlx, g->win, g->i.img, 0, 0);
		g->ray.raya -= FIELD_OF_VIEW / WIN_X;
    	if (g->ray.raya < 0)
        	g->ray.raya += 2 * M_PI;
	}
}
