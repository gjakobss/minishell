/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gjakobss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 19:28:28 by gjakobss          #+#    #+#             */
/*   Updated: 2021/12/20 19:28:30 by gjakobss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_pwd(void)
{
	char	str[999];

	getcwd(str, 999);
	printf("%s\n", str);
	return (0);
}
