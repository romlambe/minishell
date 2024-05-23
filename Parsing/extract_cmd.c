/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romlambe <romlambe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 17:07:45 by romlambe          #+#    #+#             */
/*   Updated: 2024/05/23 15:51:23 by romlambe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_quotes_state(char *curr_char, int i, t_sm handle_quote)
{
	if ((curr_char[i] == '"' || curr_char[i] == '\'') && handle_quote == reg)
	{
			if (curr_char[i] == '"')
				handle_quote = dquote;
			else
				handle_quote = quote;
	}
	else if (curr_char[i] == '"' && handle_quote == dquote)
	{
		handle_quote = reg;
	}
	else if (curr_char[i] == '\'' && handle_quote == quote)
	{
		handle_quote = reg;
	}
	printf("%d\n", handle_quote);
}

t_token	*extract_cmd(t_token **token, char *input)
{
	size_t	i;
	t_sm	handle_quote;

	i = 0;
	handle_quote = reg;
	// printf("input i:%c\n", input[i]);
	while (input[i])
	{
		set_quotes_state(input, i, quote);
		while(input[i] == ' ' || input[i] == '\t')
			i++;
		if (!input[i])
			break;
		if (input[i] == ' ' || input[i] == '\t') //si reg = 0 (pas de quote ou quotes fermees)
			i++;
		else if (ft_strncmp(input + i, ">>", 2) == 0 && handle_quote == 0)
			i = tokenize_append(token, input , i);
		else if (ft_strncmp(input + i, ">", 1) == 0 && handle_quote == 0)
			i = tokenize_output(token, input, i);
		else if (ft_strncmp(input + i, "<<", 2) == 0 && handle_quote == 0)
			i = tokenize_here_doc(token, input, i);
		else if (ft_strncmp(input + i, "<", 1) == 0 && handle_quote == 0)
			i = tokenize_input(token, input, i);			
		else if (ft_strncmp(input + i, "|", 1) == 0 && handle_quote == 0)
			i = tokenize_pipe(token, i);
		else if (ft_strncmp(input + i, ">>", 2) == 0 && handle_quote == 1)
			i = tokenize_arg(token, input, i);
		else if (ft_strncmp(input + i, ">", 1) == 0 && handle_quote == 1)
			i = tokenize_arg(token, input, i);
		else if (ft_strncmp(input + i, "<<", 2) == 0 && handle_quote == 1)
			i = tokenize_arg(token, input, i);
		else if (ft_strncmp(input + i, "<", 1) == 0 && handle_quote == 1)
			i = tokenize_arg(token, input, i);			
		else if (ft_strncmp(input + i, "|", 1) == 0 && handle_quote == 1)
			i = tokenize_arg(token, input, i);
		else if (ft_strncmp(input + i, ">>", 2) == 0 && handle_quote == 2)
			i = tokenize_arg(token, input, i);
		else if (ft_strncmp(input + i, ">", 1) == 0 && handle_quote == 2)
			i = tokenize_arg(token, input, i);
		else if (ft_strncmp(input + i, "<<", 2) == 0 && handle_quote == 2)
			i = tokenize_arg(token, input, i);
		else if (ft_strncmp(input + i, "<", 1) == 0 && handle_quote == 2)
			i = tokenize_arg(token, input, i);			
		else if (ft_strncmp(input + i, "|", 1) == 0 && handle_quote == 2)
			i = tokenize_pipe(token, i);
		else
			i = tokenize_arg(token, input, i);
		// else
			// i++;
	}
	return (*token);
}


