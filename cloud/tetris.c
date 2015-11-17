#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>

int s1;
int accept_list[5];

struct sockaddr_in addr;

socklen_t len = sizeof(addr);
char buf[BUFSIZ];
char read_buf[BUFSIZ];
char str1[128]={0};
int select_ret;
struct timeval t_val = {0, 1000};
fd_set fds, readfds;
int pid;

/*定数*/
/*ブロック幅縦・横*/
#define BLOCK_HEIGHT 4
#define BLOCK_WIDTH 4

/*スコアボード用に横幅をさらに拡大*/
/*フィールド幅縦・横*/
#define FIELD_HEIGHT 23
#define FIELD_WIDTH 21

/*消去ブロック探索開始点*/
/*底辺１９から・左３から*/
#define SEARCH_START_Y 19
#define SEARCH_START_X 3

/*関数プロトタイプ宣言*/
void my_init_var(void);
void my_init_var2(void);
void my_op_scene(void);
void my_make_block(void);
void my_gameover(void);
void my_get_key(char *,int);
void my_make_field(void);
void my_make_collision_field(void);
void my_init_field(void);
void my_save_field(void);
void my_freeze_block(void);
void my_search_line(void);
void my_draw_field(void);
void my_turn_right(void);
void my_turn_left(void);
void my_clear_field(void);
void my_timer(void);
/*My関数プロとタイプ宣言*/
//static int game_server(int);

/*グローバル変数*/
int first_block[BLOCK_HEIGHT][BLOCK_WIDTH];
int block[BLOCK_HEIGHT][BLOCK_WIDTH];
int stage[FIELD_HEIGHT][FIELD_WIDTH];
int field[FIELD_HEIGHT][FIELD_WIDTH];
int collision_field[FIELD_HEIGHT][FIELD_WIDTH];

/*ブロック７種類×４パターン*/
int blocks[28][16] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
  {0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

  {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
  {0,1,1,0,0,0,1,0,0,1,0,0,0,1,1,1},
  {0,0,1,0,1,1,1,0,0,1,1,0,0,1,0,0},
  {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
  
  {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
  {0,1,1,0,1,1,1,0,0,0,1,0,0,1,0,0},
  {0,1,0,0,0,0,1,0,0,1,1,0,0,1,1,1},
  {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,1,1,0,0,0,0,1,0,1,1,0,0},
  {0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
  {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0},

  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,1,1,0,1,0,0,0,0,1,1},
  {0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
  {0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0},

  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0},
  {0,1,1,0,1,1,1,0,1,1,0,0,1,1,1,0},
  {0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0},
  
  {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
  {0,1,0,0,1,1,1,1,0,1,0,0,1,1,1,1},
  {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0}
};

int block_index_x[4] = {0,4,8,12};
int block_index_y[7] = {0,4,8,12,16,20,24};

int fall;
int side;
int gameover_flag;
int block_flag;
int block_flag2;
int freeze_flag;
int fall_point;
int ran;
int ran2;
int turn_point;
int score;
int lv;

void game_server()
{
  struct sockaddr_in client;
  socklen_t len = sizeof(client);
  int client_sock = accept(s1, (struct sockaddr *)&client, &len);
  if(client_sock != NULL){
    int j = 0;
    while(j < FD_SETSIZE && accept_list[j] != NULL) j++;
    if(j != FD_SETSIZE){
      FD_SET(client_sock, &readfds);
      accept_list[j] = client_sock;
      printf("accept\n");
    }else{
      printf("no empty\n");
    }
  }else{
    printf("accept error\n");
  }
}

/*メイン*/
//int main(){
int tetris(int socket){
  if(socket > 0) close(socket);/* when forked game, closed previous socket */
  
  my_init_var();
  my_init_field();
  my_draw_field();
  //  my_op_scene();

  FD_ZERO(&readfds);
  FD_SET(s1, &readfds);
  FD_SET(accept_list[0], &readfds);
  
  while(1){
    memcpy(&fds, &readfds, sizeof(fd_set));
    select_ret = select(s1+accept_list[0]+1, &fds, NULL, NULL, &t_val);
    if(select_ret != 0){
      if(FD_ISSET(s1, &fds)){
	game_server();
      }
      if(FD_ISSET(accept_list[0], &fds)){
	read(accept_list[0],read_buf,BUFSIZ);
      }
    }

    if(gameover_flag == 0){
      my_make_block();
      my_gameover();
      my_get_key(read_buf, accept_list[0]);
      my_make_field();
      my_init_field();
      my_freeze_block();
      my_draw_field();
      my_clear_field();
      my_timer();
    }
    else{
      printf("gameover\n");
      break;
    }
    fall++;
  }
  return 0;
}

/*グローバル変数初期化・開始時のみ*/
void my_init_var(){
  fall = 0;
  side = 6;
  gameover_flag = 0;
  block_flag = 0;
  block_flag2 = 0;
  freeze_flag = 0;
  fall_point = 0;
  ran = 0;
  ran2 = 0;
  turn_point = 400;
  score = 0;
  lv = 1;
}

/*グローバル変数初期化・ブロック再生成時*/
void my_init_var2(){
  fall = 0;
  side = 6;	
  block_flag2 = 0;
  freeze_flag = 0;
  fall_point = 0;
  turn_point = 400;
}

/*オープニング*/
void my_op_scene(){
  int any_key;

  printf("TETRIS\n");
  strcpy(buf, "TETRIS\n");
  write(accept_list[0], buf, sizeof(buf));
  printf("press any key to start");
  strcpy(buf, "press any key to start");
  write(accept_list[0], buf, sizeof(buf));
  
  /* FD_ZERO(&readfds); */
  /* FD_SET(accept_list[0], &readfds); */
  /* memcpy(&fds, &readfds, sizeof(fd_set)); */
  /* select(accept_list[0]+1, &fds, NULL, NULL, NULL); */
  
  //  any_key = getch();
  //if(any_key == 0 || any_key == 224)any_key = getch();
}	

/*ブロック生成*/
/*２段階にブロックを生成*/
/*最初に作ったブロックを「first_block[4][4]」に保存して*/
/*そのまま「block[4][4]」に「first_block[4][4]」を移し替える*/
/*そのあと続けてブロックを作って「first_block[4][4]」に保存する*/
/*次からは「first_block[4][4]」→「block[4][4]」*/
/*ブロック生成→「first_block[4][[4]」の流れを繰り返す*/
void my_make_block(){
  int x,y;
	
  if(block_flag == 0){
    srand((unsigned)time(NULL));
    ran = (rand()%7);
    for(y=0;y<BLOCK_HEIGHT;y++){
      for(x=0;x<BLOCK_WIDTH;x++){
	first_block[y][x] = blocks[block_index_y[ran] + y][block_index_x[0] + x];
      }
    }
    block_flag = 1;
  }
  
  if(block_flag2 == 0){
    srand((unsigned)time(NULL));
    ran2 = ran;
    ran = (rand()%7);
    for(y=0;y<BLOCK_HEIGHT;y++){
      for(x=0;x<BLOCK_WIDTH;x++){
	block[y][x] = first_block[y][x];
	first_block[y][x] = blocks[block_index_y[ran] + y][block_index_x[0] + x];
      }
    }
    block_flag2 = 1;
  }
}

/*ゲームオーバー判定*/
void my_gameover(){
  int x,y;
  
  my_make_collision_field();
  
  for(y=0;y<BLOCK_HEIGHT;y++){
    for(x=0;x<BLOCK_WIDTH;x++){
      if(block[y][x] != 0){
	if(collision_field[fall + y][side + x] != 0){
	  gameover_flag++;
	}
      }
    }
  }
}

/*キー入力*/
void my_get_key(char *key, int sock){
  int pid;
  int x,y;
  int side_flag = 0;
  int fall_flag = 0;	
  
  my_make_collision_field();
      
  switch(atoi(key)){
  case 'a':
    for(y=0;y<BLOCK_HEIGHT;y++){
      for(x=0;x<BLOCK_WIDTH;x++){
	if(block[y][x] != 0){
	  if(collision_field[fall + y][side + (x - 1)] != 0){
	    side_flag++;
	  }
	}
      }
    }
    if(side_flag == 0){
      side--;
    }
    break;
  case 'd':
    for(y=0;y<BLOCK_HEIGHT;y++){
      for(x=0;x<BLOCK_WIDTH;x++){
	if(block[y][x] != 0){
	  if(collision_field[fall + y][side + (x + 1)] != 0){
	    side_flag++;
	  }
	}
      }
    }
    if(side_flag == 0){
      side++;
    }
    break;
  case 's':
    while(fall_flag == 0){
      for(y=0;y<BLOCK_HEIGHT;y++){
	for(x=0;x<BLOCK_WIDTH;x++){
	  if(block[y][x] != 0){
	    if(collision_field[fall + (y + 1)][side + x] != 0){
	      fall_flag++;
	    }
	  }
	}
      }
      if(fall_flag == 0){
	fall++;
      }
    }
    break;
  case 'f': /* fork game */
    sprintf(str1,"FORK");
    strcpy(buf, str1);
    write(accept_list[0], buf, sizeof(buf));
    if( (pid=fork()) <0 ) {
      perror("fork");
      return 1;
    } else if(pid==0) {
      tetris(sock); //fork at new port
    }
    break;
  case 0x48:
    my_turn_right();
    break;
  case 'z':
    my_turn_left();
    break;
  default:
    break;
  }
}
/* } */
/* } */


/*「field」に「block」と「stage」を登録*/
void my_make_field(){
  int i,j,x,y;
  for(y=0;y<BLOCK_HEIGHT;y++){
    for(x=0;x<BLOCK_WIDTH;x++){
      field[y+fall][x+side] = block[y][x];
    }
  }
  for(i=0;i<FIELD_HEIGHT;i++){
    for(j=0;j<FIELD_WIDTH;j++){
      field[i][j] = field[i][j] + stage[i][j];      
    }
  }
}

/*当たり判定用フィールド作成*/
void my_make_collision_field(){
  int i,j;

  for(i=0;i<FIELD_HEIGHT;i++){
    for(j=0;j<FIELD_WIDTH;j++){
      collision_field[i][j] = stage[i][j];
      collision_field[i][0] = 9;
      collision_field[i][1] = 9;
      collision_field[i][2] = 9;
      collision_field[20][j] = 9;
      collision_field[21][j] = 9;
      collision_field[22][j] = 9;
      collision_field[i][13] = 9;
      collision_field[i][14] = 9;
      collision_field[i][15] = 9;
      collision_field[i][16] = 9;
      collision_field[i][17] = 9;
      collision_field[i][18] = 9;
      collision_field[i][19] = 9;
      collision_field[i][20] = 9;
    }
  }	
}

/*「field」の端に壁「9」を登録*/
/*スコアボード等作成*/
/*スコアボードに使う文字表記を文字コードで書く*/
void my_init_field(){
  int i,j,x,y;
  int dispsc[4] = {0};
  
  for(i=0;i<FIELD_HEIGHT;i++){
    for(j=0;j<FIELD_WIDTH;j++){
      field[i][0] = 9;
      field[i][1] = 9;
      field[i][2] = 9;
      field[20][j] = 9;
      field[21][j] = 9;
      field[22][j] = 9;
      field[i][13] = 9;
      field[i][14] = 9;
      field[i][15] = 9;
    }
  }
  
  for(i=0;i<FIELD_HEIGHT;i++){
    for(j=16;j<FIELD_WIDTH;j++){
      field[i][j] = 19;
    }
  }
  
  for(y=0;y<BLOCK_HEIGHT;y++){
    for(x=0;x<BLOCK_WIDTH;x++){
      field[y+2][x+16] = first_block[y][x] + 10;
    }
  }
  
  if(score < 10){
    dispsc[0] = 10;
    dispsc[1] = 10;
    dispsc[2] = 10;
    dispsc[3] = score;
  }
  else if(score < 100){
    dispsc[0] = 10;
    dispsc[1] = 10;
    dispsc[2] = score / 10;
    dispsc[3] = score - ((score / 10) * 10);
  }
  else{
    dispsc[0] = 10;
    dispsc[1] = 10;
    dispsc[2] = 9;
    dispsc[3] = 9;
  }
  
  /*NEXT*/
  field[1][16] = 78;
  field[1][17] = 69;
  field[1][18] = 88;
  field[1][19] = 84;
  
  /*SC*/
  field[8][16] = 10;
  field[8][17] = 10;
  field[8][18] = 83;
  field[8][19] = 67;
  field[9][16] = dispsc[0];
  field[9][17] = dispsc[1];
  field[9][18] = dispsc[2];
  field[9][19] = dispsc[3];
  
  /*LV*/
  field[11][16] = 10;
  field[11][17] = 10;
  field[11][18] = 76;
  field[11][19] = 86;
  field[12][16] = 10;
  field[12][17] = 10;
  field[12][18] = 10;
  field[12][19] = lv;
}

/*ブロック固定判定*/
/*後半ブロック固定を２段階に*/
/*一回目は固定せずにその落下地点だけ保存して「fall」を戻す*/
/*次も同じ落下地点だったら固定*/
void my_freeze_block(){
  int x,y;
  
  my_make_collision_field();
  
  for(y=0;y<BLOCK_HEIGHT;y++){
    for(x=0;x<BLOCK_WIDTH;x++){
      if(block[y][x] != 0){
	if(collision_field[fall + (y + 1)][side + x] != 0){
	  freeze_flag++;
	}
      }
    }
	}
  if(freeze_flag != 0){
    if(fall_point == fall){
      my_search_line();
      my_save_field();
      my_init_var2();
    }
    else{
      freeze_flag = 0;
      fall_point = fall;
      fall--;
    }
  }
}

/*ブロック消去判定*/
void my_search_line(){
  int i,j;
  int zero_count = 0;
  int clear_flag = 0;
  int clear_lines_point[4] = {0};
  int clear_line_index = 0;
  int remain_lines_point[20] = {0};
  int remain_line_index = 0;
  
  for(i=SEARCH_START_Y;i>=SEARCH_START_Y-19;i--){
    for(j=SEARCH_START_X;j<SEARCH_START_X+10;j++){
      if(field[i][j] == 0){
	zero_count++;
      }
    }
    if(zero_count == 0){
      clear_flag++;
      clear_lines_point[clear_line_index] = i;
      clear_line_index++;
      score++;
    }
    else{
      remain_lines_point[remain_line_index] = i;
      remain_line_index++;
      zero_count = 0;
    }
  }
  
  if(clear_flag != 0){
    for(clear_line_index=0;clear_line_index<4;clear_line_index++){
      if(clear_lines_point[clear_line_index] != 0){
	for(j=SEARCH_START_X;j<SEARCH_START_X+10;j++){
	  field[clear_lines_point[clear_line_index]][j] = 0;
	}
      }
    }
    my_draw_field();
    my_timer();
    
    remain_line_index = 0;
    for(i=SEARCH_START_Y;i>=SEARCH_START_Y-19;i--){
      for(j=SEARCH_START_X;j<SEARCH_START_X+10;j++){
	field[i][j] = field[remain_lines_point[remain_line_index]][j];
      }
      remain_line_index++;
    }		
  }
}

/*ブロック固定時にフィールドをステージに保存*/
void my_save_field(){
  int i,j;
  for(i=0;i<FIELD_HEIGHT;i++){
    for(j=0;j<FIELD_WIDTH;j++){
      stage[i][j] = field[i][j];
    }
  }
}

/*画面をクリアしてから完成した「field」を表示*/
/*後半部分スコアボード表示*/
/*文字コード65(A)以上が来たら文字コードとして「%c」で表示*/
void my_draw_field(){
  int i,j;
  system("clear");
  for(i=0;i<FIELD_HEIGHT-2;i++){
    for(j=2;j<14;j++){
      if(field[i][j] == 9){
	printf("■ ");
	sprintf(str1,"■ ");
	strcpy(buf, str1);
	write(accept_list[0], buf, sizeof(buf));
      }else if(field[i][j] == 1){
	printf("□ ");
	sprintf(str1,"□ ");
	strcpy(buf, str1);
	write(accept_list[0], buf, sizeof(buf));
      }else if(field[i][j] == 2){
	printf("■ ");
	sprintf(str1,"■ ");
	strcpy(buf, str1);
	write(accept_list[0], buf, sizeof(buf));
      }else{
	printf("  ");
	sprintf(str1,"  ");
	strcpy(buf, str1);
	write(accept_list[0], buf, sizeof(buf));
      }
    }
    for(j=16;j<21;j++){
      if(field[i][j] >= 65){
	printf(" %c",field[i][j]);
	sprintf(str1," %c", field[i][j]);
	strcpy(buf, str1);
	write(accept_list[0], buf, sizeof(buf));
      }else if(field[i][j] < 10){
	printf(" %d",field[i][j]);
	sprintf(str1," %d", field[i][j]);
	strcpy(buf, str1);
	write(accept_list[0], buf, sizeof(buf));
      }else if(field[i][j] == 19){
	printf("■ ");
	sprintf(str1,"■ ");
	strcpy(buf, str1);
	write(accept_list[0], buf, sizeof(buf));
      }else if(field[i][j] == 11){
	printf("□ ");
	sprintf(str1,"□ ");
	strcpy(buf, str1);
	write(accept_list[0], buf, sizeof(buf));
      }
      else{
	printf("  ");
	sprintf(str1,"  ");
	strcpy(buf, str1);
	write(accept_list[0], buf, sizeof(buf));
      }
    }
    printf("\n");
    sprintf(str1,"\n");
    strcpy(buf, str1);
    write(accept_list[0], buf, sizeof(buf));
  }
}

/*右回転*/
void my_turn_right(){
  int x,y;
  int turn_flag = 0;
  int turn_block[4][4] = { 0 };
  
  turn_point++;
  
  my_make_collision_field();
  
  for(y=0;y<BLOCK_HEIGHT;y++){
    for(x=0;x<BLOCK_WIDTH;x++){
      turn_block[y][x] = blocks[block_index_y[ran2] + y][block_index_x[turn_point % 4] + x];
    }
  }
  
  for(y=0;y<BLOCK_HEIGHT;y++){
    for(x=0;x<BLOCK_WIDTH;x++){
      if(turn_block[y][x] != 0){
	if(collision_field[fall + y][side + x] != 0){
	  turn_flag++;
	}
      }
    }
  }
  
  if(turn_flag == 0){
    for(y=0;y<BLOCK_HEIGHT;y++){
      for(x=0;x<BLOCK_WIDTH;x++){
	block[y][x] = turn_block[y][x];
      }
    }
  }
  else{
    turn_point--;
  }
}

/*左回転*/
void my_turn_left(){
  int x,y;
  int turn_flag = 0;
  int turn_block[4][4] = { 0 };
  
  turn_point--;
  
  my_make_collision_field();
  
  for(y=0;y<BLOCK_HEIGHT;y++){
    for(x=0;x<BLOCK_WIDTH;x++){
      turn_block[y][x] = blocks[block_index_y[ran2] + y][block_index_x[turn_point % 4] + x];
    }
  }
  
  for(y=0;y<BLOCK_HEIGHT;y++){
    for(x=0;x<BLOCK_WIDTH;x++){
      if(turn_block[y][x] != 0){
	if(collision_field[fall + y][side + x] != 0){
	  turn_flag++;
	}
      }
    }
  }
  
  if(turn_flag == 0){
    for(y=0;y<BLOCK_HEIGHT;y++){
      for(x=0;x<BLOCK_WIDTH;x++){
	block[y][x] = turn_block[y][x];
      }
    }
  }
  else{
    turn_point++;
  }
}

/*フィールドクリア*/
void my_clear_field(){
  int i,j;
  for(i=0;i<FIELD_HEIGHT;i++){
    for(j=0;j<FIELD_WIDTH;j++){
      field[i][j] = 0;
    }
  }
}

/*時間稼ぎ*/
/*消去ライン数に応じてレベル・スピード変化*/
void my_timer(){
  int x,y;
  int max;
  if(score < 90){
    lv = (score / 10) + 1;
    max = 10000 - (score * 100);
  }
  else{
    lv = 9;

    max = 1000;
  }
  for(y=0;y<max;y++){
    for(x=0;x<max;x++){
    }
  }
}

int main(int argc,char *argv[])
{
  int port = PORT_NO;

  if( argc<2 ) {
    port = PORT_NO;
  } else {
    port = atoi(argv[1]);
  }
  printf("port no. = %d\n", port);
  s1 = server_socket_procedure(port);
  
  tetris(-1);

  return 0;
}
