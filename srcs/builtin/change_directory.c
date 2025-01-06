/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:47:17 by cshingai          #+#    #+#             */
/*   Updated: 2025/01/06 16:54:27 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	change_directory(t_envp **env_list, char **path)
{
	char	old_pwd[PATH_MAX];
	char	pwd[PATH_MAX];
	char	*new_path;
	new_path = *path;
	if (*path == NULL)
		new_path = ft_getenv("HOME", *env_list);
	else if (ft_strcmp(path[0], "~") == 0)
		new_path = ft_getenv("HOME", *env_list);
	else if (ft_strcmp(path[0], "-") == 0)
		new_path = ft_getenv("OLDPWD", *env_list);
	if (check_path(new_path, path) == 1)
		return (1);
	getcwd(old_pwd, PATH_MAX);
	if (chdir(new_path) == -1)
	{
		perror(new_path);
		return (1);
	}
	getcwd(pwd, PATH_MAX);
	update_pwd(env_list, old_pwd, pwd);
	return (0);
}

void	update_pwd(t_envp **env_list, char *old_pwd, char *pwd)
{
	if (old_pwd)
		change_env_value("OLDPWD", old_pwd, env_list);
	if (pwd)
		change_env_value("PWD", pwd, env_list);
}

int	check_path(char *new_path, char **path)
{
	if (new_path == NULL)
	{
		ft_printf_fd(STDERR_FILENO,
			"cd: %s: Missing file or directory\n", new_path);
		return (1);
	}
	if (check_num_path(path) == 1)
	{
		ft_printf_fd(STDERR_FILENO, "cd: Too many arguments\n");
		return (1);
	}
	return (0);
}

int	check_num_path(char **path)
{
	int	idx1;

	idx1 = 0;
	while (path[idx1])
		idx1++;
	if (idx1 > 1)
		return (1);
	return (0);
}
