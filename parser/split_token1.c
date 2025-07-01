/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:20:50 by kozcan            #+#    #+#             */
/*   Updated: 2025/06/24 12:22:06 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// is_delimiter: Token sınır karakterleri olan '|', '<', '>' için 1 döner, aksi halde 0
int	is_delimiter(char c)
{
	char *del = "|<>";

	while (*del)
	{
		if (c == *del)
			return (1); // Karakter delimiter ise
		del++;
	}
	return (0);
}

// ft_spt1: Tek karakterli veya çift karakterli delimiter tespit edildiğinde
// is_token flagini ve token sayısını günceller, str işaretçisini skip kadar ilerletir
void	ft_spt1(char **str, int *is_token, int *count, int skip)
{
	if (*is_token)
	{
		*is_token = 0; // Önceki word token sona erdi
		(*count)++;
	}
	(*count)++;       // Delimiter token için sayacı artır
	(*str) += skip;   // İşaretçiyi skip kadar ilerlet
}

// ft_spt2: Çift karakterli delimiter ('>>' veya '<<') bulunduğunda
// önceki birikmiş karakterleri token olarak ekler, ardından delimiter tokenı ekler
// tokens dizisine kopyalar ve gerekli indeksleri günceller
void	ft_spt2(char ***tokens, int *i, int *tok_len, char **str)
{
	// Birikmiş word tokenı varsa önce ekle
	if (*tok_len > 0)
	{
		(*tokens)[*i] = malloc((*tok_len + 1) * sizeof(char));
		ft_strncpy((*tokens)[*i], (*str) - *tok_len, *tok_len);
		(*tokens)[*i][*tok_len] = '\0';
		(*i)++;
	}
	// Çift karakterli delimiter tokenı ekle
	(*tokens)[*i] = malloc(3 * sizeof(char));
	ft_strncpy((*tokens)[*i], (*str), 2);
	(*tokens)[*i][2] = '\0';
	(*i)++;
	// İşaretçiyi 2 karakter ilerlet
	(*str) += 2;
	*tok_len = 0; // Word uzunluğunu sıfırla
}

// count_tokens: Verilen str içinde kaç token olduğunu sayar
// Delimiter veya word tokenlarını saymak için is_token flagini kullanır
int	count_tokens(char *str)
{
	int count = 0;
	int is_token = 0;

	// Karakter karakter dolaşarak token sınırlarını say
	while (*str)
	{
		if (strncmp(str, ">>", 2) == 0 || strncmp(str, "<<", 2) == 0)
			ft_spt1(&str, &is_token, &count, 2); // Çift delimiter
		else if (is_delimiter(*str))
			ft_spt1(&str, &is_token, &count, 1);   // Tek delimiter
		else
		{
			is_token = 1; // Word karakteri
			str++;
		}
	}
	if (is_token)
		count++; // Son word token için
	return (count);
}

// ft_split_tok: str içindeki word ve delimiter tokenlarını ayırıp
// dinamically allocate edilmiş char* dizisi olarak döndürür
char	**ft_split_tok(char *str)
{
	int token_count = count_tokens(str); // Toplam token sayısını al
	char **tokens = malloc((token_count + 1) * sizeof(char *));
	int index = 0;
	int token_length = 0;

	// Str boyunca ilerle ve tokenları oluştur
	while (*str)
	{
		if (strncmp(str, ">>", 2) == 0 || strncmp(str, "<<", 2) == 0)
			ft_spt2(&tokens, &index, &token_length, &str);
		else if (is_delimiter(*str))
			ft_spt3(&tokens, &index, &token_length, &str); // Tek delim handled elsewhere
		else
		{
			token_length++; // Word karakteri sayısını artır
			str++;
		}
	}
	// Son birikmiş word tokenı varsa ekle
	if (token_length > 0)
		ft_spt4(&tokens, &index, &token_length, &str);
	// NULL ile sonlandır
	tokens[index] = NULL;
	return (tokens);
}
