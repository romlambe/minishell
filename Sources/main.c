/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romlambe <romlambe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:14:33 by jeguerin          #+#    #+#             */
/*   Updated: 2024/06/04 18:19:55 by romlambe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*read_input(t_minishell *minishell)
{
	char	*input;

	(void)minishell;
	input = readline("Tarpin_de_Minishell>");
	if (input == NULL)
	{
		free(input);
		printf("exit\n");
		// free_tab(minishell->env);
		minishell->last_exit_status = EXIT_FAILURE;
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp(input, ":", 1) == 0 || ft_strncmp(input, "!", 1) == 0)
	{
		input = ft_strdup("");
		return (input);
	}
	add_history(input);
	return (input);
}

// int	main(int argc, char **argv, char **env)
// {
// 	char			*input;
// 	t_minishell		exit_code;
// 	t_token			*token;
// 	t_clean_token	*clean_token;
// 	t_final_token	*final_token;
// 	t_minishell		*minishell;

// 	token = NULL;
// 	(void)env;
// 	minishell = (t_minishell *)malloc(sizeof(t_minishell));
// 	manage_signals(minishell);
// 	exit_code.last_exit_status = 0;
// 	clean_token = (t_clean_token *)malloc(sizeof(t_clean_token));
// 	final_token = (t_final_token *)malloc(sizeof(t_final_token));
// 	if (argc != 1 || argv[1])
// 		return (perror("Wrong nb of args\n"), 1);
// 	minishell->env = realloc_env(env);
// 	if (env == NULL)
// 		return (perror("Realloc env. failed\n"), 1);
// 	while (1)
// 	{
// 		input = read_input(minishell);
// 		if (ft_strlen(input) == 0 || ft_isspace(input) == 1)
// 		{
// 			rl_on_new_line();
// 			free(input);
// 			continue ;
// 		}
// 		token = extract_cmd(&token, input);
// 		t_token	*head = token;
// 		clean_chevron(token);
// 		clean_spaces1(token);
// 		manage_node(token);
// 		clean_spaces2(token);
// 		if (verif_pipe(token) == 1)
// 		{
// 			rl_on_new_line();
// 			free(input);
// 			while (token)
// 			{
// 				free(token->content);
// 				token = token->next;
// 			}
// 			continue ;
// 		}
// 		token = head;
// 		clean_token = copy_lst(token);
// 		print_clean_lst(clean_token);
// 		test_redirection_input(clean_token);
// 		final_token = final_clean_node(clean_token);
// 		get_var_of_env(final_token);
// 		remove_quote(final_token);
// 		// check_line(&final_token, minishell, &exit_code);
// 		execute_commands_with_pipes_and_redirections(&final_token,
// 			minishell, &exit_code);
// 		free_that_lst(&token);
// 		if (clean_token)
// 			free_that_clean_lst(&clean_token);
// 		if (final_token)
// 			free_that_final_lst(&final_token);
// 		free(input);
// 	}
// 	free_tab(minishell->env);
// 	free(minishell);
// 	return (0);
// }

int	main(int argc, char **argv, char **env)
{
	char			*input;
	t_minishell		exit_code;
	t_token			*token;
	t_token			*head;
	t_clean_token	*clean_token;
	t_final_token	*final_token;
	t_minishell		*minishell;

	token = NULL;
	clean_token = NULL;
	final_token = NULL;
	(void)env;
	minishell = (t_minishell *)malloc(sizeof(t_minishell));
	manage_signals(minishell);
	minishell->last_exit_status = 0;
	if (argc != 1 || argv[1])
		return (perror("Wrong nb of args\n"), 1);
	minishell->env = realloc_env(env);
	if (minishell->env == NULL)
		return (perror("Realloc env. failed\n"), 1);
	while (1)
	{
		input = read_input(minishell);
		if (ft_strlen(input) == 0 || ft_isspace(input) == 1)
		{
			rl_on_new_line();
			free(input);
			continue ;
		}
		token = extract_cmd(&token, input);
		head = token;
		// if (clean_chevron(token) == 1)
		// {
		// 	printf("bash: %s error\n", input);
		// 	rl_on_new_line();
		// 	free(input);
		// 	free_that_lst(&token);
		// 	continue ;
		// }
		clean_spaces1(token);
		manage_node(token);
		clean_spaces2(token);
		if (verif_pipe(token) == 1)
		{
			rl_on_new_line();
			free(input);
			while (token)
			{
				free(token->content);
				token = token->next;
			}
			continue ;
		}
		token = head;
		clean_token = copy_lst(token);
		test_redirection_input(clean_token);
		final_token = final_clean_node(clean_token);
		get_var_of_env(final_token);
		remove_quote(final_token);
		execute_commands_with_pipes_and_redirections(&final_token,
			minishell, &exit_code);
		free_that_lst(&token);
		if (clean_token)
			free_that_clean_lst(&clean_token);
		if (final_token)
			free_that_final_lst(&final_token);
		free(input);
	}
	free_tab(minishell->env);
	free(minishell);
	return (0);
}