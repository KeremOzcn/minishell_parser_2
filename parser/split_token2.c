/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:22:14 by kozcan            #+#    #+#             */
/*   Updated: 2025/06/24 12:23:17 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ft_strncpy: src'den n+1 karakteri dest'e kopyalar,
// src kısmen veya tamamen kısa olsa da dest'i '\0' ile doldurur.
char	*ft_strncpy(char *dest, char *src, int n)
{
	int i = 0;

	// src dizisini veya n karakterini kopyala
	while (src[i] && i <= n)
	{
		dest[i] = src[i];
		i++;
	}
	// Geri kalan dest pozisyonlarını '\0' ile doldur
	while (i <= n)
	{
		dest[i++] = '\0';
	}
	return (dest);
}

// ft_spt3: Tek karakterli delimiter ('|', '<', '>') bulunduğunda
// birikmiş word tokenı ekler, sonra delimiter tokenını ekler ve işaretçiyi ilerletir.
void	ft_spt3(char ***tokens, int *i, int *tok_len, char **str)
{
	// Eğer word uzunluğu varsa önce o substring'i token olarak ekle
	if (*tok_len > 0)
	{
		(*tokens)[*i] = malloc((*tok_len + 1) * sizeof(char));
		ft_strncpy((*tokens)[*i], (*str) - *tok_len, *tok_len);
		(*tokens)[*i][*tok_len] = '\0';
		(*i)++;
	}
	// Tek karakter delimiter token oluştur
	(*tokens)[*i] = malloc(2 * sizeof(char));
	ft_strncpy((*tokens)[*i], (*str), 1);
	(*tokens)[*i][1] = '\0';
	(*i)++;
	// İşaretçiyi bir karakter ileri götür ve token uzunluğunu sıfırla
	(*str)++;
	*tok_len = 0;
}

// ft_spt4: Döngü sonunda kalan word uzunluğu varsa
// son word tokenını ekler.
void	ft_spt4(char ***tokens, int *i, int *tok_len, char **str)
{
	// Birikmiş word tokenı ekle
	(*tokens)[*i] = malloc((*tok_len + 1) * sizeof(char));
	ft_strncpy((*tokens)[*i], (*str) - *tok_len, *tok_len);
	(*tokens)[*i][*tok_len] = '\0';
	(*i)++;
}

// free_tokens: malloc ile oluşturulmuş char** dizisinin
// her bir stringini ve diziyi kendisini free eder.
void	free_tokens(char **tokens)
{
	int i = 0;
	if (tokens)
	{
		while (tokens[i] != NULL)
			free(tokens[i++]);
		free(tokens);
	}
}

// ft_setenv: Mevcut ev (envp) dizisini kopyalar,
// yeni bir char** arr yaratır ve her entry'yi strdup ile kopyalar.
char	**ft_setenv(char **ev)
{
	int i = 0;
	// ev dizisindeki satır sayısını bul
	while (ev[i])
		i++;
	// i+1 elemanlı yeni dizi allocate et (sonunda NULL için)
	char **arr = malloc((i + 1) * sizeof(char *));
	arr[i] = NULL;
	// Diziyi ters sırada kopyala
	while (i--)
		arr[i] = ft_strdup(ev[i]);
	return (arr);
}
