/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:04:25 by kozcan            #+#    #+#             */
/*   Updated: 2025/06/24 12:07:12 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ft_check_tokens: Yönlendirme ve pipe tokenlarından sonra
// geçerli bir argüman veya komut olmadığında hatayı yakalar.
// mini->head listesini dolaşır, eğer token >=2 (redir) ise
// bir sonraki token yoksa ya da o da bir operatörse hata basar.
int	ft_check_tokens(t_global *mini)
{
	t_lexer *tmp = mini->head;

	while (tmp)
	{
		// tmp->token >= 2 anlamı: REDIR_OUT, REDIR_IN, HEREDOC veya APPEND
		if (tmp->token >= 2 && tmp->next && tmp->next->token)
		{
			// Sonraki token da bir operatörse sözdizimi hatası
			ft_putstr_fd("syntax error near unexpected token\n", 2);
			g_global.error_num = 258; // Bash uyumlu exit kodu
			return (0);
		}
		else if (tmp->token >= 2 && !tmp->next)
		{
			// Yönlendirme operatöründen sonra hiçbir token yoksa hata
			ft_putstr_fd("syntax error near unexpected token\n", 2);
			g_global.error_num = 258;
			return (0);
		}
		tmp = tmp->next; // Sonraki tokena geç
	}
	return (1); // Hata bulunmadı
}

// ft_check_pipe: Ardışık, başta veya sonda pipe kullanımı
// durumunda sözdizimi hatası verir.
int	ft_check_pipe(t_global *mini)
{
	t_lexer *tmp = mini->head;

	while (tmp)
	{
		// Pipe tokenı ve önceki token da pipe ise hata
		if (tmp->token == 1 && tmp->prev && tmp->prev->token == 1)
		{
			ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
			g_global.error_num = 258;
			return (0);
		}
		// Pipe tokenı ve ya önceki veya sonraki token yoksa hata
		else if (tmp->token == 1 && (!tmp->prev || !tmp->next))
		{
			ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
			g_global.error_num = 258;
			return (0);
		}
		tmp = tmp->next;
	}
	return (1); // Hata yok
}

// ft_check_error: Hem pipe hem de token hatalarını birleştirir,
// herhangi biri 0 dönerse genel hata 1 döner.
int	ft_check_error(t_global *mini)
{
	// ft_check_pipe veya ft_check_tokens fonksiyonlarından biri başarısızsa
	// 1 (hata) döndür, aksi halde 0 (başarılı)
	return (!ft_check_pipe(mini) || !ft_check_tokens(mini));
}
