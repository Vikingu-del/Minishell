/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes_dollar.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eseferi <eseferi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 18:42:39 by eseferi           #+#    #+#             */
/*   Updated: 2024/04/11 13:17:30 by eseferi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_dollar_and_join(t_data *data, char *s, int *i, char *result)
{
	int		start;
	char	*temp;

	start = *i;
	temp = expand_dollar(data, s, i);
	if (!temp)
	{
		temp = ft_substr(s, start, *i - start);
		if (temp)
			result = ft_strjoin_double_free(result, temp);
	}
	else
		result = ft_strjoin_double_free(result, temp);
	return (result);
}

char	*expand_quotes(t_data *data, char *s)
{
	int		i;
	char	*result;
	char	*temp;

	i = -1;
	result = NULL;
	while (s[++i])
	{
		if (s[i] == '\"')
			result = process_dquote(data, s, &i, result);
		else if (s[i] == '\'')
			result = process_squote(s, &i, result);
		else if (s[i] == '$')
			result = expand_dollar_and_join(data, s, &i, result);
		else
		{
			temp = ft_substr(s, i, 1);
			if (temp)
				result = ft_strjoin_double_free(result, temp);
		}
	}
	return (result);
}

char	*expand_double_quotes(t_data *data, char *s, int *i, char *result)
{
	char	*temp;

	while (s[++(*i)] != '\"')
	{
		if (s[*i] == '$')
			result = expand_dollar_and_join(data, s, i, result);
		else
		{
			temp = ft_substr(s, *i, 1);
			if (temp)
				result = ft_strjoin_double_free(result, temp);
		}
	}
	return (result);
}

char	*expand_single_quotes(char *s, int *i, char *result)
{
	char	*temp;

	temp = NULL;
	while (s[++(*i)] != '\'')
	{
		temp = ft_substr(s, *i, 1);
		if (temp)
			result = ft_strjoin_double_free(result, temp);
	}
	return (result);
}

char	*expand_dollar(t_data *data, char *s, int *i)
{
	t_envir	*temp;
	int		len;
	int		j;

	temp = NULL;
	if (!s[*i + 1] || s[*i + 1] == '\"')
		return (ft_strdup("$"));
	if (s[*i + 1] == '?')
	{
		*i += 1;
		return (ft_itoa(data->exit_status));
	}
	j = *i + 1;
	while (is_valid_env_char(s[j]) && s[j] != ' ' && s[j] != '\0')
		j++;
	len = j - *i - 1;
	temp = find_envir_variable(data, &s[*i + 1], len);
	if (temp && !ft_strncmp(temp->var_name, &s[*i + 1], len))
	{
		*i = j - 1;
		return (ft_strdup(temp->var_value));
	}
	else
		*i = j - 1;
	return (ft_strdup(""));
}
