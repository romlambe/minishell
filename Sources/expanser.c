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


//TEST GPT
// Si j'ai une var. d'env qui n'existe pas il ne faut pas que le echo ecrive la var 
// => a voir dans la built-in directement 


// char *extract_var(char *str)
// {
//     size_t i = 0;
//     size_t start = 0;
//     size_t len = 0;
//     char *var_env;
//     char *result;

//     while (str[i])
//     {
//         if (str[i] == '$')
//         {
//             start = ++i;  // Passe le caractère '$'
//             while (str[i] && str[i] != ' ' && str[i] != '$')
//             {
//                 len++;
//                 i++;
//             }
//             var_env = strndup(str + start, len);
//             result = getenv(var_env);
//             free(var_env);
//             if (result == NULL)
//             {
//                 free(result);
//                 result = strdup("\n");
//                 // printf("\n");
//                 // return NULL;
//             }
//             return strdup(result);  // Return a copy of the result
//         }
//         i++;
//     }
//     printf("Can't get the var. of env.\n");
//     return NULL;
// }



// void get_the_var_of_env(t_final_token *node)
// {
//     t_final_token *tmp = node;
//     char *var;
//     size_t i;
//     int len;
//     char *temp;
//     char *final;

//     while (tmp)
//     {
//         i = 0;
//         while (tmp->content[i])
//         {
//             while (tmp->content[i] == ' ')
//                 i++;
//             // set_quotes_state(tmp->content, i, &state_machine);
//             if (tmp->content[i] == '$')
//             {
//                 len = len_of_var_of_env(tmp->content + i);
//                 var = extract_var(tmp->content + i);
//                 if (var == NULL)
//                     return;
                
//                 if (i == 0)
//                 {
//                     final = ft_strdup(var);
//                     tmp->content = realloc(tmp->content, strlen(final) + 1);
//                     strcpy(tmp->content, final);
// 					printf("bash: %s: Is a directory\n", final);
//                 }
//                 else
//                 {
//                     temp = strndup(tmp->content, i);
//                     final = malloc(strlen(temp) + strlen(var) + strlen(tmp->content + i + len) + 1);
//                     strcpy(final, temp);
//                     strcat(final, var);
//                     strcat(final, tmp->content + i + len + 1);
//                     free(tmp->content);
//                     tmp->content = final;
//                     free(temp);
//                 }
//                 free(var);
//             }
//             i++;
//         }
//         tmp = tmp->next;
//     }
// }

// int len_of_var_of_env(char *content)
// {
//     int i = 0;
//     int len = 0;

//     while (content[i])
//     {
//         if (content[i] == '$')
//         {
//             i++;
//             while (content[i] && content[i] != ' ' && content[i] != '$')
//             {
//                 len++;
//                 i++;
//             }
//             return len;  // Return the length immediately after finding it
//         }
//         else 
//             i++;
//     }
//     return len;
// }


// // Enlever le niveau de quote 

char   *remove_first_level_quote(char *content)
{
    int     i;
    int     j;
    int     single_quote;
    int     double_quote;
    char    *temp;

    i = 0;
    j = 0;
    single_quote = 0;
    double_quote = 0;
    temp = (char *)malloc(ft_strlen(content) + 1);
    if (!temp)
    {
        perror("malloc");
        return NULL;
    }
    while (content[i])
    {
        if (content[i] == '\'' && !double_quote)
            single_quote = !single_quote;
        else if (content[i] == '"' && !single_quote)
            double_quote = !double_quote;
        else
            temp[j++] = content[i];
        i++;
    }
    temp[j] = 0;
    return (temp);
}

void    remove_quote(t_final_token *token)
{
    while(token)
    {
        token->content = remove_first_level_quote(token->content);
        token = token->next;
    }
}

// /*
// Expanser
// */

// // int main()
// // {
// //   char  *str;

// // //   str = "echo """"pourquoi'''' \"''toi''\"";
// // //   str = "echo '\"""pourquoi\"""' \"''toi''\"";
// //   // str = "echo";
// //   // echo(str);
// //   str = "echo -n \"""cat'''' \"'cat'\" 'cat'";
// // //   str = "echo '-n' 'cat' cat cat";
// //   // echo(str);
// // //   str = "echo cat cat \"''''cat''''\"";
// //   // echo(str);
// // //   str = "echo \"''''Hello' 'World''''\' \"'''cat'''\" cat";
// // //   str = "echo cat";
// // //   builtin_echo(str);
// // // str = "\"''cat\"'' ok";
// // //   str = clean_quote(str);
// //   str = handle_quotes(str);
// //   printf("new_str : %s\n", str);
// //   return (0);
// // }

// /*
// GESTION DES GUILLEMETS : 
// - Sans guillemets : echo hello world
// La commande est interprétée comme trois tokens distincts : echo, Hello, World.
// - Avec simple quotes : echo 'Hello World'
// La commande est interprétée comme deux tokens : echo et Hello World.
// - Avec double quotes : echo "Hello World"
// La commande est également interprétée comme deux tokens : echo et Hello World.
// */

int len_of_var_of_env(char *str) {
    int len = 0;
    while (str[len] && (isalnum(str[len]) || str[len] == '_')) {
        len++;
    }
    return len;
}

char *extract_var(char *str) {
    size_t len = len_of_var_of_env(str + 1);
    char *var = (char *)malloc(len + 2);
    if (var == NULL) {
        return NULL;
    }
    strncpy(var, str, len + 1);
    var[len + 1] = '\0';
    return var;
}

void get_the_var_of_env(t_final_token *node) {
    t_final_token *tmp = node;
    char *var;
    size_t i;
    int len;
    char *temp;
    char *final;
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (tmp) {
        i = 0;
        while (tmp->content[i]) {
            if (tmp->content[i] == '\'' && !in_double_quote) {
                in_single_quote = !in_single_quote;
            } else if (tmp->content[i] == '\"' && !in_single_quote) {
                in_double_quote = !in_double_quote;
            }

            if (tmp->content[i] == '$' && !in_single_quote) {
                len = len_of_var_of_env(tmp->content + i + 1);
                var = extract_var(tmp->content + i);
                if (var == NULL) {
                    return;
                }
                
                char *env_value = getenv(var + 1); // skip the '$' character
                if (!env_value) {
                    env_value = ""; // treat undefined variables as empty
                }

                if (i == 0) {
                    final = strdup(env_value);
                    tmp->content = realloc(tmp->content, strlen(final) + 1);
                    strcpy(tmp->content, final);
                } else {
                    temp = strndup(tmp->content, i);
                    final = malloc(strlen(temp) + strlen(env_value) + strlen(tmp->content + i + len + 1) + 1);
                    strcpy(final, temp);
                    strcat(final, env_value);
                    strcat(final, tmp->content + i + len + 1);
                    free(tmp->content);
                    tmp->content = final;
                    free(temp);
                }
                free(var);
            }
            i++;
        }
        tmp = tmp->next;
    }
}