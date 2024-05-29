/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romlambe <romlambe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:14:33 by jeguerin          #+#    #+#             */
/*   Updated: 2024/05/29 11:03:27 by romlambe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*read_input(void)
{
	char	*input;

	input = readline(">$");
	if (input == NULL) // Manage ctrl + d bce this is not a signal (ascii).
	{
		free(input);
		exit(EXIT_FAILURE);
	}
    if (ft_strncmp(input, ":", 1) == 0 || ft_strncmp(input, "!", 1) == 0)
    {
        input = ft_strdup("");
        return (input);
    }
	// if (ft_strlen(input) <= 0)
	// {
	// 	perror("Input is empty\n");
	// 	free(input);
	// 	exit(0);
	// }
	add_history(input);
	return (input);
}

int		main(int argc, char **argv, char **env)
{
	char			*input;
	// char			*result;
    t_minishell		exit_code;
	t_token			*token;
	t_clean_token 	*clean_token;
	t_final_token	*final_token;
	t_minishell		*minishell;

	token = NULL;
	(void)env;
	minishell = (t_minishell *)malloc(sizeof(t_minishell));
	manage_signals();
    exit_code.last_exit_status = 0;
	clean_token = (t_clean_token *)malloc(sizeof(t_clean_token));
	final_token = (t_final_token *)malloc(sizeof(t_final_token));
	if (argc != 1 || argv[1])
		return (perror("Wrong nb of args\n"), 1);
    minishell->env = realloc_env(env);
    // if (env == NULL)
	// 	return (perror("Realloc env. failed\n"), 1);
	while (1)
	{
		input = read_input();
        if (ft_strlen(input) == 0 || ft_isspace(input) == 1)
        {
            rl_on_new_line();
            free(input);
            continue ;
        }
		// result = managing_quotes(input);
		// result = clean_quote(input);
		token = extract_cmd(&token, input); // condition si input vide
		t_token *head = token;
		clean_chevron(token);
		clean_spaces1(token);
		// printf("%s|\n", token->content);
		manage_node(token);
		clean_spaces2(token);
		// check_directory(token);
		if (verif_pipe(token) == 1)
        {
            rl_on_new_line();
            free(input);
			while(token)
			{
				free(token->content);
				token = token->next;
			}
            continue ;
        }
		token = head;
		// printf("token = %s\n", token->content);
		clean_token = copy_lst(token);
		print_clean_lst(clean_token);
		test_redirection_input(clean_token);
		final_token = final_clean_node(clean_token);
		// check_line(&clean_token, env, &exit_code);

		get_the_var_of_env(final_token);
		remove_quote(final_token);
		execute_commands_with_pipes_and_redirections(&final_token, minishell, &exit_code);
		free_that_lst(&token);
		free_that_clean_lst(&clean_token);
		free_that_final_lst(&final_token);
		//gerer les builtins car si je mets un espace pb
		//pb sur pwd pour le moment
		free(input);
	}
	free_tab(minishell->env);
	// free(minishell);
	return (0);
}
