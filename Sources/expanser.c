/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:01:08 by marvin            #+#    #+#             */
/*   Updated: 2024/04/19 12:01:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
MANAGE QUOTES
*/

// 1. Trouver l'indice du prochain guillemet fermant
// 2. Extraire la partie avant le guillemet ouvrant
// 3. Extraire la partie entre les guillemets
// 4. Extraire la partie après le guillemet fermant
// 5. Concaténer les parties sans les guillemets
// 6. Mettre à jour l'indice pour passer le contenu entre guillemets

/*
ConvertPathToAbsolute
*/

int	is_absolute_path(char **cmd)
{
	size_t	i;
	size_t	is_absolute;

	is_absolute = 0;
	i = 0;
	while (cmd[0][i])
	{
		if (cmd[0][0] == '/' && cmd[0][1])
		{
			is_absolute = 1;
			break ;
		}
		else
			break ;
		i++;
	}
	return (is_absolute);
}

// Maybe not useful for the cmd line.
// TO CHECK : when we put just "//"
int	is_relative_path(char **cmd)
{
	size_t	i;
	size_t	is_absolute;

	is_absolute = 0;
	if (cmd[1] != NULL)
	{
		i = 0;
		while (cmd[1][i])
		{
			if (cmd[1][0] == '/' && cmd[1][1])
			{
				is_absolute = 1;
				break ;
			}
			else
				break ;
			i++;
		}
	}
	return (is_absolute);
}

// TESTER SUR MAC pour les ".." et quelles est la racine
// que l'on doit protéger ? + Si ça fonctionne ou non !
// Nécessaire pour les built-ins => BEST to run.
char	*relative_to_absolute_path(char **cmd)
{
	char	cwd[1024];
	char	*partial_path;
	char	*final_path;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("Can't get the new path\n");
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(cmd[1], "..") == 0
		&& ft_strcmp(cwd, getenv("HOME")) == 0)
		return (perror("Can't go higher than the root\n"), NULL);
	partial_path = ft_strjoin(cwd, "/");
	final_path = ft_strjoin(partial_path, cmd[1]);
	free(partial_path);
	return (final_path);
}
/*
ReplaceEnvVars
*/
// How to replace the var. in the list ?
// Should we send the node->content ?
// How do we change the value of the node->content ?

// char	*extract_var(char *str)
// {
// 	size_t	i;
// 	int		len;
// 	char	*var_env;
// 	char	*result;

// 	i = 0;
// 	len = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '$')
// 		{
// 			while (str[i] || str[i] != ' ')
// 				len++;
// 			i++;
// 			var_env = ft_substr(str + i, 0, ft_strlen(str + i));
// 			result = getenv(var_env);
// 			if (result == NULL)
// 				return (perror("This var. is not existing in the env.\n"),
// 					free(var_env), NULL);
// 			return (free(var_env), result);
// 		}
// 		i++;
// 	}
// 	perror("Can't get the var. of env.");
// 	return (NULL);
// }

char *extract_var(char *str)
{
    size_t i = 0;
    size_t start = 0;
    size_t len = 0;
    char *var_env;
    char *result;

    while (str[i])
    {
        if (str[i] == '$')
        {
            start = ++i;  // Passe le caractère '$'
            while (str[i] && str[i] != ' ')
            {
                len++;
                i++;
            }
            var_env = ft_substr(str, start, len);
            result = getenv(var_env);
            free(var_env);
            if (result == NULL)
            {
                perror("This var. is not existing in the env.\n");
                return NULL;
            }
            return result;
        }
        i++;
    }
    perror("Can't get the var. of env.");
    return NULL;
}



int	check_var(t_final_token *node)
{
	t_final_token	*tmp;
	size_t	i;

	tmp = node;
	while (tmp)
	{
		i = -1;
		while (tmp->content[++i])
		{
			if (tmp->content[i] == '"')
			{
				while (tmp->content[i])
				{
					if (is_var_env(tmp->content[i]) == 0)
						return (0);
					i++;
				}
			}
		}
		tmp = tmp->next;
	}
	return (1);
}


// enfait j'suuis trop con g oublie de laisser le echo
// regarder quand j'ai plusieurs variables d'environnement + pk echo ne l'ecrit pas
// si $ sans cmd avant -> preciser que c'est un directory.
void	get_the_var_of_env(t_final_token *node)
{
    t_final_token *tmp = node;
    char *var;
    size_t i;
    int len;
	char *temp;
	char *final;

    while (tmp)
    {
        i = 0;
        while (tmp->content[i])
        {
            while (tmp->content[i] == ' ')
                i++;
            if (tmp->content[i] == '$')
            {
				if (tmp->content[0] == '$')
				{
					len = len_of_var_of_env(tmp->content);
					var = extract_var(tmp->content);
					if (var == NULL)
						return ;
               		 if (tmp->content[i + len + 1] == 0)
					{
						printf("bash:  %s: Is a directory\n", var);
                    	tmp->content = ft_strdup(var);
					}
                	else
					{
						final = ft_strdup(var);
                   		tmp->content = ft_strjoin(tmp->content + i + len + 1, final);
					}
					// tmp = tmp->next;
				}
				else 
				{
					temp = ft_strndup(tmp->content, i);
					// printf("temp = %s\n", temp);
                	len = len_of_var_of_env(tmp->content + i);
                	var = extract_var(tmp->content + i);
                	if (var == NULL)
                	    return ;
                	// printf("var = %s\n", var);
                // replace_var_of_env(tmp->content, var, i);
                	if (tmp->content[i + len + 1] == 0)
					{
						final = ft_strjoin(temp, var);
						// printf("final = %s\n", final);
                	    tmp->content = ft_strdup(final);
					}
                	else
					{
						final = ft_strjoin(temp, var);
               	    	tmp->content = ft_strjoin(final, tmp->content + i + len - 2);
					}
					// tmp = tmp->next;
				}
                // printf("var d'env changee :%s \n", tmp->content);

            }
            i++;
        }
        tmp = tmp->next;
    }
    // perror("Can't get this var.\n");
}

// char	*get_the_var_of_env(t_final_token *node)
// {
// 	t_final_token	*current = node;
// 	char			*var;
// 	size_t			i;
// 	int				len;
// 	char			*temp;

// 	while (current)
// 	{
// 		i = 0;
// 		while (current->content[i])
// 		{
// 			while(current->content[i] == ' ')
// 				i++;
// 			if (current->content[i] == '$')
// 			{
// 				temp = ft_strndup(current->content, i);
// 				len = 
// 			}
// 		}
// 	}
// }


void	replace_var_of_env(char *content, char *var, int i)
{
	//gerer le cas des quotes ou non avec l'enum 

	// while (content[i] || content[i] != ' ')
	// 	i++;
	if (content[i] == 0)
		content = ft_strdup(var);
	else
		content = ft_strjoin(var, content + i);
	printf("var d'env changee :%s \n", content);

}

int		len_of_var_of_env(char *content)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (content[i])
	{
		if (content[i] == '$')
		{
			i++;
			while (content[i] && content[i] != ' ' && content[i] != '$')
			{
				len++;
				i++;
			}
		}
		else 
			i++;
	}
	return (len);
}

/*
Expanser
*/

// int main()
// {
//   char  *str;

// //   str = "echo """"pourquoi'''' \"''toi''\"";
// //   str = "echo '\"""pourquoi\"""' \"''toi''\"";
//   // str = "echo";
//   // echo(str);
//   str = "echo -n \"""cat'''' \"'cat'\" 'cat'";
// //   str = "echo '-n' 'cat' cat cat";
//   // echo(str);
// //   str = "echo cat cat \"''''cat''''\"";
//   // echo(str);
// //   str = "echo \"''''Hello' 'World''''\' \"'''cat'''\" cat";
// //   str = "echo cat";
// //   builtin_echo(str);
// // str = "\"''cat\"'' ok";
// //   str = clean_quote(str);
//   str = handle_quotes(str);
//   printf("new_str : %s\n", str);
//   return (0);
// }

/*
GESTION DES GUILLEMETS : 
- Sans guillemets : echo hello world
La commande est interprétée comme trois tokens distincts : echo, Hello, World.
- Avec simple quotes : echo 'Hello World'
La commande est interprétée comme deux tokens : echo et Hello World.
- Avec double quotes : echo "Hello World"
La commande est également interprétée comme deux tokens : echo et Hello World.
*/
