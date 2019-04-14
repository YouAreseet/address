#include <stdio.h>

int i_condition_log = 0;

void menu_user()
{
    system("clear");

    printf(
    "************************************************************\n"
    "**********                                        **********\n"
    "**********                                        **********\n"
    "**********                WELCOME                 **********\n"
    "**********                                        **********\n"
    "**********                                        **********\n"
    "************************************************************\n"
    "                                                            \n"
    "***       1 Log in               2 retrieve              ***\n"
    "***                                                      ***\n"
    "***       3 List Username!       4 Exit!                 ***\n"
    "                                                            \n"
    "  ****      Hello, sir!  What can I do for you ?      ****  \n");

}


void menu_address()
{
    system("clear");

    printf(
    "************************************************************\n"
    "**********                                        **********\n"
    "**********                                        **********\n"
    "**********                WELCOME                 **********\n"
    "**********                                        **********\n"
    "**********                                        **********\n"
    "************************************************************\n"
    "                                                            \n"
    "***       1 List Information!    2 Add Information!      ***\n"
    "***       3 Delete Information!  4 Modify Information!   ***\n"
    "***       5 Search Information   6 Exit!                 ***\n"
    "                                                            \n"
    "  ****      Hello, sir!  What can I do for you ?      ****  \n");

}


void user_handler(int fd)
{
	char choice[32] = {0};
	
	while(1)
	{
		menu_address();
		
		scanf("%s", choice);
		
		switch(choice[0])
		{
			case '1':
				show_info(fd);
				break;
			case '2':
				add_info(fd);
				break;
			case '3':
				del_info(fd);
				break;
			case '4':
				update_info(fd);
				break;
			case '5':
				search_info(fd);
				break;
			case '6':
				exit_address(fd);
				return;
			default:
				break;
		}
	}
}


void main_handler(int fd)
{
    char choice[32] = {0};
    while(1)
    {
        menu_user();
		
		i_condition_log = 0;

		scanf("%s", choice);

		switch(choice[0])
		{
			case '1':
				log_in(fd);
				if(1 == i_condition_log)
				{
					user_handler(fd);
				}
				break;
			case '2':
				add_user(fd);
				break;
			case '3':
				show_user(fd);
				break;
			case '4':
				exit_client(fd);
			/*
			case '4':
				update_info(fd);
				break;
			case '5':
				search_info(fd);
				break;
			case '6':
				del_info(fd);
				break;
			*/
			default:
				break;

		}
    }
}
