#include<stdio.h>
#include<vector>

#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //�����X�g���[��
#include<tuple>

using namespace std;
int record_count = 0;
FILE *fp_record,fp_read_correct;
char record_directory[128];
char read_correct_directory[128];
int count_score=0;
std::vector<std::vector<int>>correct_xy_point;

std::tuple<std::vector<std::vector<int>>> read_correct_xy(int data_num) {

	//�z��̒�`�Ə�����
	std::vector<std::vector<int>>correct_xy_point;
	correct_xy_point.resize(3);
	for (int i = 0; i < 3; ++i) {
		correct_xy_point[i].resize(data_num + 1);
	}
	int i = 0;
	int j = 0;
	char *Filename_c_s = "..\\bmp\\simulation18-0115\\correct.csv";
	sprintf(read_correct_directory, "%s", Filename_c_s);

	ifstream read_correct(read_correct_directory);
	
	string str_read_correct;

	while (getline(read_correct, str_read_correct)) {
		i = 0;
		string token_read_correct;
		istringstream stream_read_correct(str_read_correct);

		while (getline(stream_read_correct, token_read_correct, ',')) {	//��s�ǂݎ��D
			if (i > 0 && j > 0) {
				int tmp_read_correct = stof(token_read_correct);			//�����𐔎��ɕϊ�
				correct_xy_point[i][j] = tmp_read_correct;

			}

			++i;
		}
		++j;
	}
	for (j = 1; j<data_num + 1; ++j) {

		printf("correct_x=%d,correct_y=%d\n", correct_xy_point[1][j], correct_xy_point[2][j]);

	}


	return std::forward_as_tuple(correct_xy_point);

}


int score(int correct_x,int correct_y,int max_x,int max_y,int image_xt,int image_yt, int data_num){
	//���_���v�Z���܂�

	int high_score_range_x = 5;		//�����̍��W�́}high_score_range�Ȃ�score 1
	int high_score_range_y = 5;
	double middle_score_range_x_percent = 0.5;	//�����̍��W����e���v���[�g�摜�T�C�Y�~middle_score_range
	double middle_score_range_y_percent = 0.5;
	int middle_score_range_x=0;
	int middle_score_range_y=0;
	int correct_score=0;
	int point_flag = 0;
	int multi_vote_flag = 0;
	int i,j;
	
	++count_score;
	printf("image_xt=%d,image_yt=%d\n", image_xt, image_yt);
	middle_score_range_x = image_xt*middle_score_range_x_percent;
	middle_score_range_y = image_yt*middle_score_range_y_percent;

	printf("middle_score_range_x=%d,middle_score_range_y=%d\n", middle_score_range_x, middle_score_range_y);

	
		//�z��̒�`�Ə�����
	//	std::vector<std::vector<int>>correct_xy_point;
	if(count_score==1){
		correct_xy_point.resize(3);
		for (int i = 0; i < 3; ++i) {
			correct_xy_point[i].resize(data_num + 1);
		}

		std::tie(correct_xy_point) = read_correct_xy(data_num);
	}
	

	correct_x = correct_xy_point[1][count_score];
	correct_y = correct_xy_point[2][count_score];
	printf("score:correct_x=%d,correct_y=%d\n", correct_x, correct_y);

	

	//�����ɓ������W�̎�

		if (max_x == correct_x && max_y == correct_y) {
			correct_score = 1;
			point_flag = 1;
		}
	
	
	//���W���}high_score_range�̎�
	if (point_flag == 0) {
		
			for (int i = correct_y - high_score_range_y; i <= correct_y + high_score_range_y; ++i) {
				for (int j = correct_x - high_score_range_x; j <= correct_x + high_score_range_x; ++j) {

					if (max_x == j && max_y == i) {
						correct_score = 2;
						point_flag = 1;
						break;
					}
				}
				if (point_flag == 1)break;
			}
		
		
	}

	//���W���e���v���[�g�摜�T�C�Y�~middle_score_range�̎�
	if (point_flag == 0) {
		
			for (int i = correct_y - middle_score_range_y; i <= correct_y + middle_score_range_y; ++i) {
				for (int j = correct_x - middle_score_range_x; j <= correct_x + middle_score_range_x; ++j) {

					if (max_x == j && max_y == i) {
						correct_score = 3;
						point_flag = 1;
						break;
					}
				}
				if (point_flag == 1)break;
			}
			
	}

	//�ǂ�ɂ����Ă͂܂�Ȃ���
	if (point_flag == 0) {
		correct_score = 4;
	}
	
	return correct_score;
}



int score_record(char date_directory[],int correct_score,int max_x, int max_y) {


	++record_count;

	if (record_count == 1) {

		
		//�o�͌��ʂ̃t�@�C�����̎w��
		char *Filename_s="score_record.csv";
		
		sprintf(record_directory, "%s%s", date_directory, Filename_s);

		
		if ((fp_record = fopen(record_directory, "w")) == NULL) {
			printf("�t�@�C���F%s���J���܂���\n", record_directory);
			exit(1);
		}
		fprintf(fp_record, "����,x,y\n");
	}
	

	switch(correct_score) {
	case 1:
		fprintf(fp_record, "��,%d,%d\n", max_x, max_y);
		break;
	case 2:
		fprintf(fp_record, "��,%d,%d\n", max_x, max_y);
		break;
	case 3:
		fprintf(fp_record, "��,%d,%d\n", max_x, max_y);
		break;
	case 4:
		fprintf(fp_record, "�~,%d,%d\n", max_x, max_y);
		break;
	default:
		printf("correct_score����������\ncorrect_score=%d\n", correct_score);
		exit(1);
		break;
	}


	//fclose(fp_record);
	return 0;
}