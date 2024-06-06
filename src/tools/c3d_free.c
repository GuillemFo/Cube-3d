/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c3d_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 22:57:58 by josegar2          #+#    #+#             */
/*   Updated: 2024/06/07 01:02:46 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// it's called if file != NULL
t_file  *c3d_free_file(t_file *file)
{
    int i;

    file->NO = ft_free(file->NO);
    file->SO = ft_free(file->SO);
    file->EA = ft_free(file->EA);
    file->WE = ft_free(file->WE);
    file->F = ft_free(file->F);
    file->C = ft_free(file->C);
    if (file->map)
    {
        i = 0;
        while (i <= file->max_y + 2)
        {
            file->map[i] = ft_free(file->map[i])
            i++;
        }
        file->map = ft_free(file->map);
    }
    // TODO: what to do with file->tmp?
    file = ft_free(file);
    return (file);
}

t_mlx   *c3d_free_win(t_mlx *win)
{
    if (win->mlx_win)
    {
        // close window
    }
    if (win->mlx)
    {
        // close mlx
    }
}

t_data  *c3d_free(t_data *data)
{
    if (data)
    {
        if (data->file)
            data->file=c3d_free_file(data->file);
        if (data->win)
            data->win=c3d_free_win(data->win);
        data = ft_free(data);
    }
    return (data);
}