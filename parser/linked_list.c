/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:16:11 by kozcan            #+#    #+#             */
/*   Updated: 2025/06/24 12:16:32 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ft_set_index: mini->head listesindeki her t_lexer düğümünün
// i alanını, liste sırasına göre yeniden atar (0'dan başlayarak).
void	ft_set_index(t_global *mini)
{
	t_lexer *temp = mini->head;
	int    i = 0;

	while (temp)
	{
		temp->i = i++;      // Her düğüme sırasıyla indeks ata
		temp = temp->next;  // Sonraki düğüme geç
	}
}

// ft_delete_node: Belirtilen index'e sahip t_lexer düğümünü bulup listeden çıkarır.
// Ardından string ve yapı serbest bırakılır, ve indeksler yeniden düzenlenir.
void	ft_delete_node(int index, t_global *mini)
{
	t_lexer *temp = mini->head;

	if (!temp)
		return; // Liste boşsa

	// Silinecek düğümü ara (temp->i == index)
	while (temp->next && temp->i != index)
		temp = temp->next;
	if (temp->i != index)
		return; // Belirtilen index bulunamadı

	// Bağlantıları güncelle
	if (temp->prev)
		temp->prev->next = temp->next;
	else
		mini->head = temp->next; // İlk düğüm siliniyorsa head güncelle

	if (temp->next)
		temp->next->prev = temp->prev;

	// Belleği temizle
	free(temp->str); // Token metnini free et
	free(temp);      // Düğüm yapısını free et

	// İndeksleri yeniden ata
	ft_set_index(mini);
}

// ft_size_cmds: mini->p_head listesindeki t_simple_cmds düğümlerinin sayısını döner.
// Pipeline'daki toplam komut sayısını bulmak için kullanılır.
int	ft_size_cmds(t_global *mini)
{
	t_simple_cmds *temp = mini->p_head;
	int           size = 0;

	while (temp)
	{
		temp = temp->next; // Sonraki komut düğümüne geç
		size++;            // Sayaç artışı
	}
	return (size);
}
