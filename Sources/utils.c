/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:27:43 by marvin            #+#    #+#             */
/*   Updated: 2024/03/22 10:27:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
En résumé, l'utilisation d'un pointeur double vous permet de modifier le 
pointeur lui-même, pas seulement la valeur à laquelle il pointe. Cela vous 
permet de réinitialiser correctement le pointeur token à NULL après avoir 
libéré la mémoire de la liste chaînée.
*/
void	free_that_lst(t_token **token)
{
	t_token	*tmp;

	if (!(*token))
	{
		perror("Lst is empty, can't free\n");
		exit(EXIT_FAILURE);
	}
	while (*token)
	{
		tmp = (*token)->next;
		ft_free((*token)->content);
		ft_free(*token);
		*token = tmp;
	}
	*token = NULL;
}

t_token	*lst_last(t_token *token)
{
	if (token == NULL)
	{
		perror("Token node is empty\n");
		exit(EXIT_FAILURE);
	}
	while (token->next)
		token = token->next;
	return (token);
}

void	add_back(t_token **token, t_token *new)
{
	t_token	*last;

	if (!token)
	{
		perror("List is empty\n");
		exit(EXIT_FAILURE);
	}
	if (!(*token))
		*token = new;
	else
	{
		last = lst_last(*token);
		last->next = new;
	}
}

void	print_env(t_minishell *minishell)
{
	size_t	i;

	i = 0;
	sort_tab(minishell->env);
	while (minishell->env[i])
	{
		printf("declare -x %s\n", minishell->env[i]);
		i++;
	}
	return ;
}

int	is_one_cmd(char *cmd)
{
	size_t	i;
	int		is_option;

	i = 0;
	is_option = 0;
	while (cmd[i])
	{
		if (cmd[i] == ' ' && ((cmd[i + 1] >= 65 && cmd[i + 1] <= 90)
				|| (cmd[i + 1] >= 97 && cmd[i + 1] <= 122)))
		{
			is_option = 1;
			return (is_option);
		}
		i++;
	}
	return (is_option);
}
