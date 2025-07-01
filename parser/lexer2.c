/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:13:53 by kozcan            #+#    #+#             */
/*   Updated: 2025/06/24 12:16:07 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ft_first_node: mini->head listesi boşsa çağrılır.
// Yeni bir t_lexer düğümü allocate eder, token tipini ve metnini ayarlar,
// bağlantıları (next, prev) NULL olarak başlatır ve indisi 0 yapar.
// Ayrıca, başlangıç karakterine göre is_quote flagini belirler.
void	ft_first_node(t_global *mini, char *str)
{
	mini->head = (t_lexer *)malloc(sizeof(t_lexer));
	if (!mini->head)
		return; // Bellek ayrılmadıysa fonksiyondan çık
	
	mini->head->next = NULL;
	mini->head->prev = NULL;
	// Token tipini belirle: PIPE, REDIR, WORD vs.
	mini->head->token = ft_find_token(str);
	// Token metnini kopyala
	mini->head->str = ft_strdup(str);
	// Sıfırıncı index
	mini->head->i = 0;
	// Eğer token bir tırnakla başlıyorsa is_quote değerine ata
	if (str[0] == '"')
		mini->head->is_quote = 2;
	else if (str[0] == '\'')
		mini->head->is_quote = 1;
	else
		mini->head->is_quote = 0;
}

// ft_last_node: mini->head listesi doluysa yeni düğümü sona ekler.
// Yeni düğüm allocate edilir, token ve metin ayarlanır,
// önceki son düğümü bulup bağlantıları günceller, indisi artırır
// ve is_quote flagini belirler.
void	ft_last_node(t_global *mini, char *str)
{
	t_lexer *new;
	t_lexer *temp;

	new = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new)
		return; // Bellek ayrılmadıysa çık
	new->next = NULL;
	// Token tipini ve metnini ayarla
	new->token = ft_find_token(str);
	new->str = ft_strdup(str);
	
	// Liste başından son düğüme kadar ilerle
	temp = mini->head;
	while (temp->next)
		temp = temp->next;
	// Bağlantıları oluştur
	new->prev = temp;
	temp->next = new;
	// İndeksi bir öncekinin indisine göre ayarla
	new->i = new->prev->i + 1;
	// is_quote flagini baş karaktere göre belirle
	if (str[0] == '"')
		new->is_quote = 2;
	else if (str[0] == '\'')
		new->is_quote = 1;
	else
		new->is_quote = 0;
}

// ft_find_token: Verilen str stringine bakarak uygun token tipini döner.
// "|" -> PIPE, ">>" -> APPEND, ">" -> REDIR_OUT,
// "<<" -> HEREDOC, "<" -> REDIR_IN, aksi halde WORD.
int	ft_find_token(char *str)
{
	if (!ft_strcmp("|", str))
		return (1); // PIPE token
	if (!ft_strcmp(">>", str))
		return (3); // REDIR_APPEND
	if (!ft_strcmp(">", str))
		return (2); // REDIR_OUT
	if (!ft_strcmp("<<", str))
		return (5); // HEREDOC
	if (!ft_strcmp("<", str))
		return (4); // REDIR_IN
	return (0);    // WORD (diğer)
}
