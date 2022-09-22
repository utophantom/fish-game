// header file declare
#include<stdio.h>
#include<stdlib.h>

struct Grid {
  char data;
  struct Grid *up;
  struct Grid *down;
  struct Grid *left;
  struct Grid *right;
  struct Grid *upLeft;
  struct Grid *upRight;
  struct Grid *downLeft;
  struct Grid *downRight;
};

int check_bound(int cur_pos) {
  if(cur_pos<0 || cur_pos>35) {
    return 36;
  }
  return cur_pos;
};

//Check when position is not left/right wall
int check_left(int cur_pos) {
  if(check_bound(cur_pos) == 36) {
    return 36;
  }
  if(cur_pos % 6 == 5) {
    return 36;
  }
  return cur_pos;
}

int check_right(int cur_pos) {
  if(check_bound(cur_pos) == 36) {
    return 36;
  }
  if(cur_pos % 6 == 0) {
    return 36;
  }
  return cur_pos;
}

char get_data(int i){
  if( (i>=7 && i<=10) || (i>=25 && i<=28) || (i==13) || (i==16) || (i==19) || (i==22) ) {
    return '2';
  }
  else if( i==14 || i==15 || i==20 || i==21) {
    return '3';
  }
  else{
    return '1'; 
  }
}


struct Grid * game_start() {
  struct Grid *ptr;
  ptr = (struct Grid*) malloc(37 * sizeof(struct Grid));
  struct Grid bad_alloc = {'*',&bad_alloc,&bad_alloc,&bad_alloc,&bad_alloc,&bad_alloc,&bad_alloc,&bad_alloc,&bad_alloc};
  ptr[36] = bad_alloc;
  
  for(int i=0; i<36; i++) {
    struct Grid grid_init;
    grid_init.data = get_data(i);
    grid_init.up = &(ptr[check_bound(i-6)]);
    grid_init.down = &(ptr[check_bound(i+6)]);
    grid_init.left = &(ptr[check_left(i-1)]);
    grid_init.right = &(ptr[check_right(i+1)]);
    grid_init.upLeft = &(ptr[check_left(i-7)]);
    grid_init.upRight = &(ptr[check_right(i-5)]);
    grid_init.downLeft = &(ptr[check_left(i+5)]);
    grid_init.downRight = &(ptr[check_right(i+7)]);
    ptr[i] = grid_init;    
  }
  return ptr;
};

void print_board(struct Grid *ptr) {
  struct Grid *cur_ptr = ptr;
  int cur_pos = 0;
  
  printf("\n   1  2  3  4  5  6");
  for(int i=0; i<6; i++) {
    printf("\n\n%i",i+1);
    for(int j=0; j<6; j++) {
      printf("  %c", cur_ptr[cur_pos].data);
      cur_pos++; 
    }
  }  
}

int check_1pt(int i) {
  if( (i>=1 && i<=4) || (i>=31 && i<=34) || (i % 6 == 0 || (i%6 == 5) )) {
    return 1;
  }
  return 0;
}

int player_init(struct Grid *ptr) {
  struct Grid *cur_ptr = ptr;
  int start_row = -1;
  int start_col = -1;
  int player_pos = -1;  
  
  //Ask player for starting position
  printf("\nWelcome! First, choose your position! \nRemember, only choose positions that contain 1 point!");
  printf("\nRow #: ");
  scanf("%i", &start_row);
  printf("\nCol #: ");
  scanf("%i", &start_col);
  
  int player_count = -1;
  for(int i=1; i<=6; i++) {
    for(int j=1; j<=6; j++) {
      player_count++;
      if(i==start_row && j==start_col){
        player_pos = player_count;
      }
    }
  }    
  cur_ptr[player_pos].data = 'P';  
  return player_pos;
}

//Random initial position for AI
int AI_init(struct Grid *ptr) {
  int AI_pos = -1;
  while( !((check_1pt(AI_pos) == 1) && (ptr[AI_pos].data != 'P')) ) { 
    AI_pos = rand() % 36;
    
  }
  ptr[AI_pos].data = 'A';
  return AI_pos;
}

//direction of move is decided base on the length of the input,
//given that the input is of the same format every time.
int move(int pos, size_t string_length, int step, struct Grid *ptr) {
  struct Grid *cur_grid = &ptr[pos];
  char char_grid;
  int i;
  char temp_char = ptr[pos].data;

  if(step == 0) {
    return 0;
  }  
  for(i=1; i<=step; i++) {      
    switch(string_length) {
      case 4:
        cur_grid = cur_grid[0].up;
        break;
      case 5:
        cur_grid = cur_grid[0].down;
        break;
      case 6:
        cur_grid = cur_grid[0].left;
        break;
      case 7:
        cur_grid = cur_grid[0].right;
        break;
      case 8:
        cur_grid = cur_grid[0].upLeft;
        break;
      case 9:
        cur_grid = cur_grid[0].upRight;
        break;
      case 10:
        cur_grid = cur_grid[0].downLeft;
        break;
      case 11:
        cur_grid = cur_grid[0].downRight;
        break;
    }
    char_grid = cur_grid[0].data;
    if(char_grid =='1' || char_grid == '2' || char_grid == '3') {
        ;
    }
    //If this func return 0, it means that the player did not move 
    //due to wrong input
    else {
        return 0; 
    }
  }
      char_grid = cur_grid[0].data;
      int res = char_grid - '0';
      ptr[pos].data = '*';
      cur_grid[0].data = temp_char;
      return res;
}

int locator(char the_who, struct Grid *ptr) {
    for(int i=0; i<36; i++) {
        if(ptr[i].data == the_who) {
            return i;
        }
    }
}

//AI find best point out of 8 direction
//First 8 mem-location are for direction(1-8), 
//next 8 are for max_point in that dir (0-3),
//last 8 are for step to reach there
int AI_optmz(int AI_pos, struct Grid *ptr) {
  int cur_max = 0;
  int best_dir;
  struct Grid *cur_grid;
  int *max_list = (int*) malloc(26 * sizeof(int));
  for(int i=0; i<8; i++) {
    cur_grid = &ptr[AI_pos];
    max_list[i] = i+1;
    max_list[i+8] = 0;
    max_list[i+16] = 0;
    for(int j=1; j<6; j++) {
      switch(i+1) {
        case 1:
          cur_grid = cur_grid[0].up;
          break;
        case 2:
          cur_grid = cur_grid[0].down;
          break;
        case 3:
          cur_grid = cur_grid[0].left;
          break;
        case 4:
          cur_grid = cur_grid[0].right;
          break;
        case 5:
          cur_grid = cur_grid[0].upLeft;
          break;
        case 6:
          cur_grid = cur_grid[0].upRight;
          break;
        case 7:
          cur_grid = cur_grid[0].downLeft;
          break;
        case 8:
          cur_grid = cur_grid[0].downRight;
          break;
      }
      if(cur_grid == &ptr[36] || cur_grid[0].data == '*' || cur_grid[0].data == 'P') {
        break;
      }

      if(cur_grid[0].data - '0' < max_list[i+8]) {
        break;
      }
      int a = cur_grid[0].data - '0';
      max_list[i+8] = cur_grid[0].data - '0';
      max_list[i+16] = j;
    }
  }
  for(int k=0; k<8; k++) {
    if(cur_max < max_list[k+8]) {
      cur_max = max_list[k+8];
      best_dir = max_list[k];
    }
  }
  return best_dir*10 + max_list[best_dir+15];
}
//check surround of player or AI stuck or not
int check_stuck(int pos, int opponent_pos, struct Grid *ptr) {
  struct Grid *cur_grid;
  int flag = 0;
  char opponent_char = ptr[opponent_pos].data;

  cur_grid = ptr[pos].up ;
  if(cur_grid[0].data == '*' ||  cur_grid[0].data == opponent_char) {
    flag += 1;
  }
  cur_grid = ptr[pos].down;
  if(cur_grid[0].data == '*' ||  cur_grid[0].data == opponent_char) {
    flag += 1;
  }
  cur_grid = ptr[pos].left;
  if(cur_grid[0].data == '*' ||  cur_grid[0].data == opponent_char) {
    flag += 1;
  }
  cur_grid = ptr[pos].right;
  if(cur_grid[0].data == '*' ||  cur_grid[0].data == opponent_char) {
    flag += 1;
  }
  cur_grid = ptr[pos].upLeft;
  if(cur_grid[0].data == '*' ||  cur_grid[0].data == opponent_char) {
    flag += 1;
  }
  cur_grid = ptr[pos].upRight;
  if(cur_grid[0].data == '*' ||  cur_grid[0].data == opponent_char) {
    flag += 1;
  }
  cur_grid = ptr[pos].downLeft;
  if(cur_grid[0].data == '*' ||  cur_grid[0].data == opponent_char) {
    flag += 1;
  }
  cur_grid = ptr[pos].downRight;
  if(cur_grid[0].data == '*' ||  cur_grid[0].data == opponent_char) {
    flag += 1;
  }
  if(flag == 8) {
    return 1;
  }
  return 0;
}

void game_end(int player_pt, int AI_pt) {
  if(player_pt > AI_pt) {
    printf("\n\n\nCONGRATULATION!!! YOU HAVE WON");
  }
  else if(player_pt < AI_pt) {
    printf("\n\n\nYou failed... AI has won...");
  }
  else{
    printf("IT IS A TIE!!");
  }
  printf("YOUR SCORE IS: %i         AI'S SCORE IS: %i\n", player_pt, AI_pt);
}


int main() {
  printf(
"----------------------------------------\n"
"    WELCOME TO HEY, THAT'S MY FISH \n"
"----------------------------------------\n"

);
    int player_pt = 1;
    int player_pos;
    int AI_pt = 1;
    int AI_pos;
    int player_stuck = 0;
    int AI_stuck = 0;
    
    struct Grid *ptr = game_start();
    print_board(ptr);
    player_pos = player_init(ptr);
    AI_pos = AI_init(ptr);

    while(!(player_stuck == 1 && AI_stuck == 1)) {
      if(check_stuck(player_pos, AI_pos, ptr) == 1) {
        player_stuck = 1;
        printf("Player stuck and cannot make a move");
        break;
      }
      else{
        printf("player not stuck");
      }
      print_board(ptr);      
      int player_incre = 0;
      player_pos = locator('P', ptr);
      AI_pos = locator('A', ptr);

      while(player_incre==0) {
        char *string;
        size_t size = 32;
        size_t string_length;
        int step = 0;
        
        string = (char*) malloc(size * sizeof(char));
        
        printf("\n It is your turn! Press ENTER and type (eg. down 3): ");
        ((getchar()) != '\n');
        string_length = getline(&string,&size,stdin);
        if(string[0] == ' ') {
          string[0] = 1;
        }
        string_length -= 1;
        
        step = string[string_length-1] - '0';

        if(string_length == 6 && (*string == 'd' || *string == 'D')) {
          string_length -= 1; 
        }       
        player_incre = move(player_pos, string_length, step, ptr);
        
        if(player_incre == 0) {
          printf("\nInvalid move, please try again.\n");
        }
      }  
      player_pt += (int) player_incre;
      print_board(ptr);
      printf("\nYour score: %i\nAI's point: %i",player_pt, AI_pt);
      printf("\nGreat move! Now is AI's turn.");
    
    // AI TURN 
      if(check_stuck(AI_pos, player_pos, ptr) != 1) {
        int temp = AI_optmz(AI_pos, ptr);
        int string_length = ((temp - (temp % 10))/10)+3;
        int step = temp % 10;

        AI_pt += move(AI_pos, string_length, step, ptr);
        printf("\n AI made a move!");
        printf("\nYour score: %i\nAI's point: %i",player_pt, AI_pt);
      }
      else{
        printf("\nAI is stuck and cannot move.");
        AI_stuck = 1;
      }
    }
    game_end(player_pt-32, AI_pt);
    exit(0);  
  return 0;

};
 
