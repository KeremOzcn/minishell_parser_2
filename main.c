/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:03:29 by kozcan            #+#    #+#             */
/*   Updated: 2025/07/01 18:20:18 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_globals	g_global;

// minishell_loop: Kullanıcıdan komut alıp işleyen ana döngü fonksiyonu.
void	minishell_loop(char *line, t_global *mini)
{
	while (1)
	{
		// Global test flaglerini her döngü başında sıfırla
		g_global.test1 = 0;
		g_global.a = 0;
		g_global.test2 = 0;

		// Prompt göster, kullanıcı girdisini oku
		line = readline("minishell$ ");
		if (line == NULL)
			// EOF (Ctrl+D) veya okuma hatası durumunda çık
			exit(1);

		// Girilen komut boş değilse geçmişe kaydet
		if (line[0])
			add_history(line);

		// Lexical analiz: satırı tokenlara böl
		ft_init_lexer(mini, line);

		// Söz dizimi hatası var mı kontrol et
		if (ft_check_error(mini))
		{
			// Hata varsa lexer belleğini boşalt ve döngüye devam et
			ft_deallocate_lexer(mini);
			free(line);
			continue ;
		}

		// Burada değişken genişletme yapılabilir (henüz implement edilmedi)
		// ft_expand(mini);

		// Parser: token listesini komut yapılarına dönüştür
		ft_init_parser(mini);

		// Komut çalıştırma adımı (executor) bu fonksiyonda çağrılacak
		// ft_executer(mini);

		// Kullanılan bellekleri temizle
		ft_deallocate_all(mini);
		free(line);
	}
}

// main: Program başlangıç fonksiyonu
int	main(int ac, char **av, char **ev)
{
	char		*line;
	t_global	mini;

	// Parametre kontrolü: ac > 1 ise hata olarak çıkış
	if (ac > 1)
		exit(1);
	
	line = NULL;
	(void)av; // av kullanılmıyor, derleyici uyarısını önlemek için cast

	// Çevre değişkenlerini kopyala
	mini.env = ft_setenv(ev);
	mini.export = ft_setenv(ev);

	// Lexer ve parser başlatma
	mini.head = NULL;
	mini.p_head = NULL;

	// Global env pointer
	g_global.env = ev;

	// Sinyal handler'lar (isteğe bağlı)
	// signal(SIGQUIT, sigquit_handler);
	// signal(SIGINT, ft_handler);

	// Shell döngüsünü başlat
	minishell_loop(line, &mini);

	return (0);
}
