/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_log.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanmpark <hanmpark@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 17:20:10 by hanmpark          #+#    #+#             */
/*   Updated: 2022/12/19 00:57:19 by hanmpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/scholarship_logtime.h"
/*\*/
// CALCULATES TIME NOT PARSED
int	ccl_time(char **date, int itype)
{
	int		idate = -1;
	int		og_itype = itype;
	int		index = 0;
	char	stash[3];
	int		res = 0;

	while (date[++idate])
	{
		itype = og_itype;
		index = 0;
		while (index < 2)
			stash[index++] = date[idate][itype++];
		stash[index] = 0;
		res += atoi(stash);
	}
	return (res);
}

// PARSING OF LOGTIME
int	*ccl_log(char **date)
{
	int	*res;

	res = malloc(3 * sizeof(int));
	if (!res)
		return (NULL);
	res[0] = 0; // seconds
	res[1] = 0; // minutes
	res[2] = 0; // hours
	// CALCULATES SECONDS
	res[0] = ccl_time(date, 18);
	res[1] = res[0] / 60;
	res[0] %= 60;
	// CALCULATES MINUTES
	res[1] += ccl_time(date, 15);
	res[2] = res[1] / 60;
	res[1] %= 60;
	// CALCULATES HOURS
	res[2] += ccl_time(date, 12);
	return (res);
}

void	set_bnlog(int *stdlog, int *bnlog)
{
	if (bnlog[2] >= 140 && bnlog[2] <= 210)
	{
		bnlog[2] -= 140;
		printf("\033[1mBonus logtime added from previous month = %dh %dmin %ds\n\n\033[0m", bnlog[2], bnlog[1], bnlog[0]);
		printf("\033[0;36mwithout bonus = \033[4m%dh %dmin %ds\033[0m\033[1;36m\n\033[0m", stdlog[2], stdlog[1], stdlog[0]);
	}
	else if (bnlog[2] > 210)
	{
		bnlog[0] = 0;
		bnlog[1] = 0;
		bnlog[2] = 70;
		printf("\033[1mBonus logtime added from previous month = %dh %dmin %ds\n\n\033[0m", bnlog[2], bnlog[1], bnlog[0]);
		printf("\033[0;36mwithout bonus = \033[4m%dh %dmin %ds\033[0m\033[1;36m\n\033[0m", stdlog[2], stdlog[1], stdlog[0]);
	}
	else
	{
		bnlog[0] = 0;
		bnlog[1] = 0;
		bnlog[2] = 0;
		printf("\033[1;31m| No bonus logtime |\n\n\033[0m");
	}
}

int	*ccl_total_time(char **date, int *bnlog)
{
	int	*ttlog;
	int	*stdlog;

	stdlog = ccl_log(date);
	set_bnlog(stdlog, bnlog);
	ttlog = ccl_log(date);
	ttlog[0] += bnlog[0];
	if (ttlog[0] > 59)
	{
		ttlog[1] += ttlog[0] / 60;
		ttlog[0] %= 60;
	}
	ttlog[1] += bnlog[1];
	if (ttlog[1] > 59)
	{
		ttlog[2] += ttlog[1] / 60;
		ttlog[1] %= 60;
	}
	ttlog[2] += bnlog[2];
	free(bnlog);
	free(stdlog);
	return (ttlog);
}

void	check_logtime(int *stdlog, int *ttlog)
{
	if (ttlog[2] < 140)
	{
		ccl_timeleft(ttlog);
		printf("--> \033[1m%dh %dmin %ds\033[0m left\n\n", 140 - ttlog[2], ttlog[1], ttlog[0]);
	}
	else if (stdlog[2] >= 140 && stdlog[2] < 210)
	{
		stdlog[2] -= 140;
		printf("--> \033[1mYou are good for this month !\n\033[0m");
		printf("--> \033[1m%dh %dmin %ds\033[0m additional time for next month\n\n", stdlog[2], stdlog[1], stdlog[0]);
	}
	else if (stdlog[2] >= 210)
	{
		stdlog[2] = 70;
		stdlog[1] = 0;
		stdlog[0] = 0;
		printf("--> \033[1mYou are good for this month !\n\033[0m");
		printf("--> \033[1m%dh %dmin %ds\033[0m additional time for next month\n\n", stdlog[2], stdlog[1], stdlog[0]);
	}
	else
		printf("--> \033[1mYou are good for this month !\n\n\033[0m");
}
/**/