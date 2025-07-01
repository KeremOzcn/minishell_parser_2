/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:07:34 by kozcan            #+#    #+#             */
/*   Updated: 2025/06/24 12:13:49 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ft_push_back: mini->head listesine yeni token ekler.
// Eğer liste boşsa ft_first_node ile başlatır, doluysa ft_last_node ile sona ekler.
void	ft_push_back(t_global *mini, char *str)
{
	if (!mini->head)
	{
		ft_first_node(mini, str); // İlk düğümü oluştur
		return ;
	}
	ft_last_node(mini, str);      // Yeni düğümü listenin sonuna ekle
}

// ft_double_quote: Karakter dizisinde çift tırnak içindeki metni atlayarak
// kapanan tırnağa veya operatöre rastlayana kadar i indeksini artırır.
void	ft_double_quote(char *line, int *i, int *flag)
{
	*flag = 1;                     // Tırnak içindeyiz
	while (line[++*i] && *flag == 1)
	{
		if (line[*i] == '"' || line[*i] == '|' \
		|| line[*i] == '>' || line[*i] == '<')
			*flag = 0;             // Çift tırnak sonu veya operatör bulundu
	}
}

// ft_single_quote: ft_double_quote ile aynı mantıkta,
// tek tırnak (') içindeki metni işler.
void	ft_single_quote(char *line, int *i, int *flag)
{
	*flag = 1;
	while (line[++*i] && *flag == 1)
	{
		if (line[*i] == '\'' || line[*i] == '|' \
		|| line[*i] == '>' || line[*i] == '<')
			*flag = 0;             // Tek tırnak sonu veya operatör
	}
}

// ft_quote: Bir lexeme içinde açık tırnak kalıp kalmadığını kontrol eder.
// Eğer kapanmamış tırnak varsa 1 döner, aksi halde 0.
int	ft_quote(char *line)
{
	int	flag = 0;
	int	i = 0;

	while (line[i])
	{
		if (line[i] == '"')
			ft_double_quote(line, &i, &flag); // Çift tırnak bloğu atla
		else if (line[i] == '\'')
			ft_single_quote(line, &i, &flag); // Tek tırnak bloğu atla
		else
			i++;                           // Düz karakterse ilerle
	}
	return (flag);
}

// ft_init_lexer: Kullanıcıdan gelen satırı önce boşluk karakterinden bölerek lexemelere ayırır.
// Her lexeme için, açık tırnak içermiyorsa ft_split_tok ile detaylı tokenize eder,
// aksi halde boşluk bazlı split kullanır. Elde edilen tokenlar mini->head listesine eklenir.
void	ft_init_lexer(t_global *mini, char *line)
{
	char	**lexemes;
	char	**tokens;
	int		i;
	int		j;

	// Boşluk karakterine göre ilk düz split
	lexemes = ft_split(line, ' ');
	if (!lexemes)
		return;                       // Bölme başarısızsa geri dön

	i = 0;
	while (lexemes[i])
	{
		// Açık tırnak yoksa ve operatör ayrımı gerekiyorsa detaylı split
		if (!ft_quote(lexemes[i]))
			tokens = ft_split_tok(lexemes[i]);
		else
			tokens = ft_split(lexemes[i], ' '); // Aksi halde basit split

		j = 0;
		while (tokens[j])
		{
			ft_push_back(mini, tokens[j]); // Her tokenu listeye ekle
			j++;
		}
		free_tokens(tokens);           // Geçici token matrisini temizle
		i++;
	}
	free_tokens(lexemes);            // Geçici lexeme matrisini temizle
}
