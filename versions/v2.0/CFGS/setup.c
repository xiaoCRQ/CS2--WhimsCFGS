#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "synchapi.h"
#include "winnt.h"
#define USER_PATH "../../../../../../../userdata/"
#define USER_KEY "cs2_user_keys_0_slot0.vcfg"
#define USER_CONVARS "cs2_user_convars_0_slot0.vcfg"
#define MACHINE_CONVARS "cs2_machine_convars.vcfg"
#define LOCAL_CONFIG_PATH "/730/local/cfg/"
#define EXEC "+exec CFGS/init.cfg"
#define FILE_COUNT 3
#define CMD_CD "cd "
#define CMD_DIR "dir "

FILE *file_user_key, *file_user_convers, *file_machine_convers;

char *save_tin() { // ��ֵ�ַ���
  int i = 0;
  char *str = (char *)malloc(1);
  while ((str[i++] = getchar()) != '\n')
    realloc(str, i + 1);
  str[i - 1] = 0; // ��β'\0'
  return str;
}

int save(char ***str) { // ��ָ�����鸳ֵ�ַ���
  int i = 0, siz = sizeof(char *);
  *str = (char **)malloc(siz); // �����ʼ��ָ�������ڴ�
  do {
    printf("......�ַ���%d ", i);
    (*str)[i] = save_tin();                       // ��ֵ�ַ���
    *str = (char **)realloc(*str, siz * (i + 2)); // �����ַ���
  } while (*(*str)[i++]); // ���ַ�������
  realloc(*str, siz * --i); // ɾ�����������ַ�
  return i;
}

void del_qt(char *str) {
  char temp[500];
  if (*str != '\"' || str[strlen(str) - 1] != '\"')
    return;
  strcpy(temp, str + 1);
  temp[strlen(temp) - 1] = '\0';
  strcpy(str, temp);
}

int strcmp_in(char *str1, char *str2) {
  while (*str1 == *str2 && *str1 && *str2) {
    str1++;
    str2++;
  }
  if (*str2)
    return 0;
  else
    return 1;
}

void file_seek(FILE *file, int count) {
  rewind(file);
  while (count-- > 1) {
    fscanf(file, "%s");
  }
}

int file_find_str(FILE *file, char *find_str,
                  int *count) { // �ַ���λ�� | ��ʼ���ļ�λ��
  char str[500];
  if (*count < 0)
    *count = 0;
  file_seek(file, *count);

  do {
    fscanf(file, "%s", str);
    del_qt(str);
    (*count)++;
  } while (!strcmp_in(str, find_str) && !feof(file));
  if (!strcmp_in(str, find_str)) {
    rewind(file);
    *count = 0;
    return 0;
  }
  puts("---");
  printf("%s %d\n", str, *count);
  puts("---");
  file_seek(file, *count);
  return 1;
}

void file_print(FILE *file) {
  do {
    char str[100];
    fscanf(file, "%s", str);
    puts(str);
  } while (!feof(file));
}

FILE *load_file(char *path, char *file_name, char *file_restrict) {
  // path - �ļ�·�� | file_name - �ļ��� |  file_restrict - �ļ���ȡ��ʽ
  FILE *file;
  char *cmd =
      (char *)malloc(sizeof(char) * (strlen(path) + strlen(file_name) + 1));
  if (!strcmp("r", file_restrict) || !strcmp("r+", file_restrict))
    printf("......���ڶ�ȡ %s\n", file_name);
  else if (!strcmp("w", file_restrict) || !strcmp("w+", file_restrict))
    printf("......����д�� %s\n", file_name);
  else if (!strcmp("a", file_restrict) || !strcmp("a+", file_restrict))
    printf("......������� %s\n", file_name);
  else
    puts("��ʽ����");
  strcpy(cmd, path);
  strcat(cmd, file_name);
  Sleep(100);
  file = fopen(cmd, file_restrict);
  if (file)
    puts("......�ɹ�!");
  else
    puts("......ʧ��!");
  ;
  free(cmd);
  Sleep(100);
  return file;
}

void load_config() { // ��ȡ�ļ�
  char *user_name, *user_path;
  int error, file_count = 0;

  while (1) {
    char *cmd;
    error =
        system("powershell -NoProfile cls;dir ../../../../../../../userdata/");
    if (error) {
      puts("δ�ҵ�userdata�ļ��У��뽫CFGS�ļ��з���cs2��cfg�ļ������ٳ���...");
      exit(0);
    }
    puts("--------------------------�û�Ŀ¼����--------------------------");
    printf("\n����������steam����[��Ҫ����ո�][0�˳�����]: ");
    user_name = save_tin();
    if (!strcmp(user_name, "0"))
      exit(0);
    cmd = (char *)malloc(sizeof(char) *
                         (strlen(CMD_CD) + strlen(USER_PATH) +
                          strlen(user_name) + strlen(LOCAL_CONFIG_PATH) + 1));
    strcpy(cmd, CMD_CD);
    strcat(cmd, USER_PATH);
    strcat(cmd, user_name);
    strcat(cmd, LOCAL_CONFIG_PATH);
    error = system(cmd); // ����steam���Ƿ����
    free(cmd);
    if (error) {
      puts("");
      system("cls");
      puts("----------------------------------------------------------------");
      puts("δ�ҵ�steam��,������!");
      Sleep(100);
    } else
      break;
    system("cls");
  }
  putchar('\n');
  user_path =
      (char *)malloc(sizeof(char) * (strlen(USER_PATH) + strlen(user_name) +
                                     strlen(LOCAL_CONFIG_PATH) + 1));
  strcpy(user_path, USER_PATH);
  strcat(user_path, user_name);
  strcat(user_path, LOCAL_CONFIG_PATH);

  // ����key�ļ�
  file_user_key = load_file(user_path, USER_KEY, "r");
  if (file_user_key)
    file_count++;

  // ����user_convars�ļ�
  file_user_convers = load_file(user_path, USER_CONVARS, "r");
  if (file_user_convers)
    file_count++;

  // ����machine_convars�ļ�
  file_machine_convers = load_file(user_path, MACHINE_CONVARS, "r");
  if (file_machine_convers)
    file_count++;

  if (file_count == FILE_COUNT)
    puts("\n--- �Ѽ������������ļ� ---\n");
  else
    printf("\n--- ȱʧ��%d���ļ���ʹ��Ĭ������ ---\n\n",
           FILE_COUNT - file_count);

  free(user_name);
}

void config_key() {
  puts("------------------------��ʼд���λ����------------------------");
  FILE *bind = load_file("config/", "keymaps.cfg", "r+");
  puts("------------------------��λ����д��ɹ�------------------------");
  fclose(bind);
}

void config_convers_user() {
  puts("------------------------��ʼд��׼������------------------------");
  FILE *crosshair = load_file("config/crosshair/", "define.cfg", "w");
  int count = 0;
  char str[2][500];
  rewind(file_user_convers);
  while (file_find_str(file_user_convers, "cl_crosshair", &count)) {
    fscanf(file_user_convers, "%s%s", str[0], str[1]);
    del_qt(str[0]);
    del_qt(str[1]);
    fprintf(crosshair, "%s\t\t%s\n", str[0], str[1]);
    count++;
  }
  puts("------------------------׼������д��ɹ�------------------------");
  fclose(crosshair);
  rewind(file_user_convers);
}

void config_convers_machine() {
  puts("------------------------��ʼд����Ϸ����------------------------");
  puts("------------------------��Ϸ����д��ɹ�------------------------");
}

void config_to_cfg() {
  puts("......��ʼ��ȡ�����ļ�");
  if (file_user_key)
    config_key();
  else
    puts("......�������ý�ʹ��Ĭ��ֵ[ȱʧ�ļ�]");

  if (file_user_convers)
    config_convers_user();
  else
    puts("......�û����ý�ʹ��Ĭ��ֵ[ȱʧ�ļ�]");

  if (file_machine_convers)
    config_convers_machine();
  else
    puts("......��Ϸ���ý�ʹ��Ĭ��ֵ[ȱʧ�ļ�]");

  puts("......�������");
}

int autoexec_5e() {
  printf("......���5e��������\n");
  FILE *file_autoexec_5e = load_file("../", "f5eautoexec.cfg", "a");
  if (file_autoexec_5e)
    puts("......��ӳɹ�");
  else
    puts("......���ʧ��");
  fprintf(file_autoexec_5e, "\n%s\n", EXEC);
  fclose(file_autoexec_5e);
}

void autoexec() {
  printf("......�����������\n");
  FILE *file_autoexec = load_file("../", "autoexec.cfg", "a");
  if (file_autoexec)
    puts("......��ӳɹ�");
  else
    puts("......���ʧ��");
  fprintf(file_autoexec, "\n%s\n", EXEC);
  fclose(file_autoexec);
}

void start() {
  char *cmd;
  while (1) {
    system("cls");
    puts("----------------------------------------------------------------");
    printf("�Ƿ�ʼ��װ[yes/no]�� ");
    cmd = save_tin();
    if (strcmp(cmd, "yes") == 0 || strcmp(cmd, "y") == 0)
      break;
    else if (strcmp(cmd, "no") == 0 || strcmp(cmd, "n") == 0)
      exit(0);
    else {
      system("cls");
      puts("----------------------------------------------------------------");
      puts("�������������!");
    }
    free(cmd);
  }
}

int main(int argc, char *argv[]) {
  start();
  system("cls");
  puts("-------------------------����Ĭ��������-------------------------");
  load_config();   // ���������ļ�
  config_to_cfg(); // д������

  puts("------------------------�����Ϸ��������------------------------");
  // autoexec();    // ���������
  // autoexec_5e(); // ���5e������

  fclose(file_user_key);
  fclose(file_user_convers);
  fclose(file_machine_convers);
  puts("--��װ����--");
  system("pause");
  return EXIT_SUCCESS;
}
