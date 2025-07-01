/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 15:29:05 by kozcan          #+#    #+#             */
/*   Updated: 2025/06/24 12:06:30 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ft_freearr: NULL-terminated char** dizisini ve içindeki her stringi free eder
// Ardından işaretçiyi NULL'a çevirerek dangling pointer riskini önler.
void	ft_freearr(char ***str)
{
	int	i;

	i = 0;
	if (!(*str))
		return; // Boş dizi için işlem yok
	while ((*str)[i])
		free((*str)[i++]); // Her stringi free et
	free((*str));         // Dizi işaretçisini free et
	*str = NULL;          // İşaretçiyi NULL yap
}

// ft_deallocate_lexer: mini->head listesindeki tüm t_lexer düğümlerini temizler
// Her düğümün 'str' alanı free edilir, düğüm bellekten silinir.
void	ft_deallocate_lexer(t_global *mini)
{
	t_lexer	*temp;

	temp = mini->head;
	while (mini->head)
	{
		free(mini->head->str);         // Token stringini free et
		temp = mini->head->next;       // Sonraki düğümü kaydet
		free(mini->head);              // Mevcut düğümü free et
		mini->head = temp;             // İşaretçiyi ilerlet
	}
}

// ft_deallocate_lexlist: Verilen t_lexer* listesi işaretçisi üzerinden
// her düğümün içindeki str'yi free edip, düğümleri serbest bırakır.
void	ft_deallocate_lexlist(t_lexer **list)
{
	t_lexer	*temp;

	temp = (*list);
	if (!(*list))
		return; // Liste boşsa çık
	while ((*list))
	{
		free((*list)->str);            // Token metnini free et
		(*list)->str = NULL;           // İkincil riskleri azaltmak için NULL yap
		temp = (*list)->next;          // Sonraki düğümü kaydet
		free((*list));                 // Mevcut düğümü free et
		(*list) = temp;                // Liste başını ilerlet
	}
}

// ft_deallocate_parser: mini->p_head listesindeki tüm t_simple_cmds düğümlerini temizler
// Her komut düğümünde aşağıdakiler serbest bırakılır:
//  - Here-doc için ayrılan hd_file_name
//  - Yönlendirme tokenları listesi (ft_deallocate_lexlist)
//  - Argüman dizisi (ft_freearr)
// Düğümler bellekten silinir.
void	ft_deallocate_parser(t_global *mini)
{
	t_simple_cmds	*temp;

	temp = mini->p_head;
	while (mini->p_head)
	{
		if (mini->p_head->hd_file_name)
		{
			free(mini->p_head->hd_file_name); // Here-doc dosya adını temizle
			mini->p_head->hd_file_name = NULL;
		}
		ft_deallocate_lexlist(&(mini->p_head->redirections)); // Yönlendirme listesi
		ft_freearr(&(mini->p_head->str));                  // Argv dizisini temizle
		temp = mini->p_head->next;                          // Sonraki komut düğümü
		free(mini->p_head);                                 // Mevcut düğümü free et
		mini->p_head = temp;                                // İlerlet
	}
}

// ft_deallocate_all: Hem parser hem de lexer için tüm dinamik verileri temizler
// Önce parser, sonra lexer temizliği yapılır.
void	ft_deallocate_all(t_global *mini)
{
	ft_skipinit(NULL, 1);                 // Gerekirse global skip flag'ini sıfırla
	ft_deallocate_parser(mini);           // Parser yapısını temizle
	ft_deallocate_lexer(mini);            // Lexer yapısını temizle
}
