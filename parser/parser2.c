/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:19:43 by kozcan            #+#    #+#             */
/*   Updated: 2025/06/24 12:20:29 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ft_strcmp: İki stringi karakter karakter karşılaştırır.
// Farklı ilk karakterin ASCII farkını döner, yoksa 0.
int	ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	// Karakterler eşit olduğu ve her iki string sonu gelmediği sürece ilerle
	while (s1[i] && s2[i] && s1[i] == s2[i])
		++i;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

// ft_pipesize: Bir pipeline segmentindeki token sayısını sayar (PIPE dahil değil).
int	ft_pipesize(t_lexer *lexer)
{
	int i = 0;
	// PIPE tokenına gelene kadar say
	while (lexer && lexer->token != PIPE)
	{
		i++;
		lexer = lexer->next;
	}
	return i;
}

// ft_init_matrix: Her t_simple_cmds düğümü için argv dizisi oluşturur.
// mini->head içindeki token listesine göre her segmentin tokenlarını kopyalar.
void	ft_init_matrix(t_global *mini)
{
	t_lexer *temp = mini->head;
	t_simple_cmds *cmd = mini->p_head;
	int i;

	// Her komut düğümü için
	while (cmd)
	{
		i = 0;
		// ft_pipesize ile segment token sayısını al ve +1 NULL için
		cmd->str = malloc((ft_pipesize(temp) + 1) * sizeof(char *));

		// PIPE görene kadar token metinlerini argv dizisine kopyala
		while (temp && temp->token != PIPE)
		{
			cmd->str[i++] = ft_strdup(temp->str);
			temp = temp->next;
		}
		// Eğer PIPE varsa atlaya geç
		if (temp)
			temp = temp->next;
		// Argv dizisini NULL ile sonlandır
		cmd->str[i] = NULL;
		// Sonraki komut düğümüne geç
		cmd = cmd->next;
	}
}
