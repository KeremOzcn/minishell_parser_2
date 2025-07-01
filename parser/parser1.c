/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:18:03 by kozcan            #+#    #+#             */
/*   Updated: 2025/07/01 18:41:04 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ft_parser_firstnode: mini->head listesinden ilk komut segmentini ayıklar
// PIPE tokenına kadar ilerler, redirection tokenlarını yeni t_simple_cmds düğümüne ekler,
// eklenen redirectionları lexer listesinden siler ve new'ü p_head olarak atar.
void	ft_parser_firstnode(t_global *mini, t_lexer *lexer)
{
	t_simple_cmds *new_cmd;
	int             i;

	// Yeni komut düğümünü initialize et ve here-doc sayaçlarını sıfırla
	ft_skipinit(&new_cmd, 0);
	new_cmd->prev = NULL;

	// PIPE görülene kadar tokenları işle
	while (lexer && lexer->token != PIPE)
	{
		// Eğer token bir operatör (redir) ise
		if (lexer->token)
		{
			// Redirection tokenını komuta ekle
			ft_parser_lexpush_back(new_cmd, lexer->str);
			// Yönlendirme hedefini de ekle (örn. ">" sonra "file.txt")
			ft_parser_lexpush_back(new_cmd, lexer->next->str);

			// Silinecek düğümlerin indeksini al
			i = lexer->i;
			// İlerleyip iki düğümü sil
			lexer = lexer->next->next;
			ft_delete_node(i, mini);
			ft_delete_node(i, mini);

			// Bu komut için bir redirection sayısı arttır
			new_cmd->num_redirections++;
			continue ;
		}
		// Redirection değilse sıradaki tokena geç
		lexer = lexer->next;
	}
	// Yeni oluşturulan komutu p_head olarak ata
	mini->p_head = new_cmd;
}

// ft_skipinit: t_simple_cmds düğümünü hazırlar veya sayaçları resetler
// flag == 1 ise sayaç i'yi sıfırlar, flag == 0 ise yeni komut node allocate eder
void	ft_skipinit(t_simple_cmds **new, int flag)
{
	static int i = 0;
	char      *num_str;
	char      *path_prefix;

	if (flag)
	{
		// Her parser döngüsünde sayaçı sıfırla
		i = 0;
		return;
	}
	// Yeni komut düğümü allocate et ve alanları init et
	*new = (t_simple_cmds *)malloc(sizeof(t_simple_cmds));
	(*new)->next = NULL;
	(*new)->num_redirections = 0;

	// Here-doc için geçici dosya adını oluştur
	path_prefix = ft_strdup("/Users/kozcan/temps/"); // Kullanıcı yolunu güncelle
	num_str = ft_itoa(i++);
	(*new)->hd_file_name = ft_strjoin(path_prefix, num_str);
	free(num_str);
	free(path_prefix);

	// Argv ve redirections başlangıçlarını NULL yap
	(*new)->str = NULL;
	(*new)->redirections = NULL;
	(*new)->in_her = 0;
}

// ft_parser_lastnode: mini->p_head listesinin sonuna yeni komut segmenti ekler
// İlk segmentten sonra gelen segmentler için kullanılır, logic birinci ile benzer
void	ft_parser_lastnode(t_global *mini, t_lexer *lexer)
{
	t_simple_cmds *new_cmd;
	t_simple_cmds *last;
	int             i;

	// Yeni komut node'u oluştur
	ft_skipinit(&new_cmd, 0);
	// p_head listesinin son noktasına ilerle
	last = mini->p_head;
	while (last->next)
		last = last->next;

	// Redirection ve PIPE öncesi tokenları işle
	while (lexer && lexer->token != PIPE)
	{
		if (lexer->token)
		{
			ft_parser_lexpush_back(new_cmd, lexer->str);
			ft_parser_lexpush_back(new_cmd, lexer->next->str);

			i = lexer->i;
			lexer = lexer->next->next;
			ft_delete_node(i, mini);
			ft_delete_node(i, mini);

			new_cmd->num_redirections++;
			continue ;
		}
		lexer = lexer->next;
	}
	// Listenin sonuna ekle ve prev bağlantısını kur
	last->next = new_cmd;
	new_cmd->prev = last;
}

// ft_parser_pushback: İlk komut için ft_parser_firstnode, sonrakiler için ft_parser_lastnode çağırır
void	ft_parser_pushback(t_global *mini, t_lexer *lexer)
{
	if (!mini->p_head)
		ft_parser_firstnode(mini, lexer);
	else
		ft_parser_lastnode(mini, lexer);
}

// ft_init_parser: Lexer listesindeki her pipeline segmenti için t_simple_cmds oluşturur
// PIPE tokenına göre segmentleri ayırır, komutları parse_pushback ile ekler, sonra argv dizisini hazırlar
void	ft_init_parser(t_global *mini)
{
	t_lexer *lexer = mini->head;
	t_lexer *forward = lexer;

	mini->p_head = NULL; // Komut listesi başlat
	if (!lexer)
		return;

	// PIPE'lara kadar ilerleyen forward pointer ile segmentleri ayır
	while (lexer)
	{
		// forward PIPE veya sona kadar ilerle
		while (forward && forward->token != PIPE)
			forward = forward->next;
		// Eğer PIPE bulunduysa bir sonraki tokena geç
		if (forward)
			forward = forward->next;
		// Ayrılan segmenti komut listesine ekle
		ft_parser_pushback(mini, lexer);
		// İlerlet
		lexer = forward;
	}
	// Her komut node'u için argüman matrisi oluştur
	ft_init_matrix(mini);
	// Builtin init yapılacaksa burada çağrılabilir
	// ft_init_builtin(mini);
}
