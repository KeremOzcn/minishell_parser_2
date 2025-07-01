/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pushback.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:16:41 by kozcan            #+#    #+#             */
/*   Updated: 2025/07/01 13:47:32 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ft_parser_lexfirst_node: Yeni bir yönlendirme tokenı (redir) düğümü oluşturur
// temporary komut düğümünün redirections başına ekler. İlk kullanımda çağrılır.
void	ft_parser_lexfirst_node(t_simple_cmds *temporary, char *str)
{
	t_lexer *redir;
	(void)redir;
	// Bellekten t_lexer düğümü ayır
	temporary->redirections = (t_lexer *)malloc(sizeof(t_lexer));
	if (!temporary->redirections)
		return; // Allocation başarısızsa çık

	// Bağlantıları başlat
	temporary->redirections->next = NULL;
	temporary->redirections->prev = NULL;
	// Token tipini belirle (>, >>, <, <<)
	temporary->redirections->token = ft_find_token(str);
	// Token metnini kopyala
	temporary->redirections->str = ft_strdup(str);
	// İlk indeks = 0
	temporary->redirections->i = 0;
	// is_quote flagini başlangıç karakterine göre ayarla
	if (str[0] == '"')
		temporary->redirections->is_quote = 2;
	else if (str[0] == '\'')
		temporary->redirections->is_quote = 1;
	else
		temporary->redirections->is_quote = 0;
}

// ft_parser_lexlast_node: temporary->redirections listesi doluysa
// yeni bir yönlendirme düğümünü listenin sonuna ekler.
void	ft_parser_lexlast_node(t_simple_cmds *temporary, char *str)
{
	t_lexer *new;
	t_lexer *temp;

	// Yeni düğüm allocate
	new = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new)
		return;
	new->next = NULL;
	// Token tipini ve metnini ayarla
	new->token = ft_find_token(str);
	new->str = ft_strdup(str);

	// Geçici işaretçi ile listenin sonunu bul
	temp = temporary->redirections;
	while (temp->next)
		temp = temp->next;
	// Bağlantıları kur
	new->prev = temp;
	temp->next = new;
	// İndeksi önceki düğümün indisine göre belirle
	new->i = new->prev->i + 1;
	// is_quote flagini belirle
	if (str[0] == '"')
		new->is_quote = 2;
	else if (str[0] == '\'')
		new->is_quote = 1;
	else
		new->is_quote = 0;
}

// ft_parser_lexpush_back: temporary komut düğümüne gelen redirection tokenını ekler.
// Eğer redirections listesi boşsa ft_parser_lexfirst_node, değilse ft_parser_lexlast_node çağrılır.
void	ft_parser_lexpush_back(t_simple_cmds *temporary, char *str)
{
	if (!temporary->redirections)
	{
		ft_parser_lexfirst_node(temporary, str); // İlk yönlendirme
		return;
	}
	// Liste doluysa sona ekle
	ft_parser_lexlast_node(temporary, str);
}
