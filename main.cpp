#include "DxLib.h"
#include "math.h"


class Bullet {
public:
    double x; // �e�̍��Wx
    double y; // �e�̍��Wy
    double vx; // �e�̈ړ���x
    double vy; // �e�̈ړ���y
    double angle; // �e�̊p�x
    double speed; // �e�̑��x
    bool use; // �e���g�p����Ă��邩

};

const int BulletMax = 1024;
Bullet bullets[BulletMax];

void AddBullet(double x, double y , double angle , double speed) {
    for (int i = 0; i < BulletMax; i++) {
        if (!bullets[i].use) {
            Bullet* bullet = &bullets[i];
            bullet->x = x;
            bullet->y = y;
            bullet->angle =  angle / 57.29;
            bullet->speed = speed;

            // �O�p�֐��𗘗p���ړ��ʂ����߂�
            bullet->vx = cos(bullet->angle) * speed;
            bullet->vy = sin(bullet->angle) * speed;

            bullet->use = true;
            return;
        }
    }
}



void InitBullet() {
    for (int i = 0; i < BulletMax; i++) {
        Bullet* bullet = &bullets[i];
        memset(bullet, 0,sizeof(Bullet));
    }
}

void DrawBullet() {
    for (int i = 0; i < BulletMax; i++) {
        if (bullets[i].use) {
            Bullet* bullet = &bullets[i];
            int Color = GetColor(0, 0, 255); // �F���擾
            DrawCircle(bullet->x, bullet->y, 20 ,Color, TRUE);
        }
    }
}

void MoveBullet() {
    for (int i = 0; i < BulletMax; i++) {
        if (bullets[i].use) {
            Bullet* bullet = &bullets[i];
            bullet->x += bullet->vx;
            bullet->y += bullet->vy;
        }
    }
}

class Player {
public:
    double x; // �v���C���[�̍��Wx
    double y; // �v���C���[�̍��Wy
    int image; // �v���C���[�̉摜
};

Player player;

void InitPlayer() {
    player.x = 400;
    player.y = 400;
    player.image = LoadGraph("assets\\player.png");
}

void DrawPlayer() {
    // DrawGraph(player.x, player.y, player.image, TRUE);
    DrawRotaGraph(player.x, player.y, 0.25 , 0.0 , player.image ,TRUE ,FALSE);
}

// �v���C���[����̃��\�b�h
void MovePlayer() {
    char Buf[256];
    GetHitKeyStateAll(Buf);

    double moveValue = 10;
    int count = 0;

    if (Buf[KEY_INPUT_LEFT] == 1) {
        count++;
    }
    else if (Buf[KEY_INPUT_RIGHT] == 1) {
        count++;
    }
    if (Buf[KEY_INPUT_UP] == 1) {
        count++;
    }
    else if (Buf[KEY_INPUT_DOWN] == 1) {
        count++;
    }

    if (count == 1) {
        moveValue = 10;
    }
    else if (count == 2) {
        // �O�����̒藝�𗘗p���ĎΕӂ̒l���v�Z
        moveValue = hypot(moveValue, moveValue);
    }

    if (Buf[KEY_INPUT_LEFT] == 1) {
        player.x -= moveValue;
    }
    else if (Buf[KEY_INPUT_RIGHT] == 1) {
        player.x += moveValue;
    }
    if (Buf[KEY_INPUT_UP] == 1) {
        player.y -= moveValue;
    }
    else if (Buf[KEY_INPUT_DOWN] == 1) {
        player.y += moveValue;
    }
}

// �����蔻��̃��\�b�h
void Collusion(){
    // �e�̍��W�ƃv���C���[�̍��W�������ł����OUT
    for (int i = 0; i < BulletMax; i++) {
        if (bullets[i].use) {
            Bullet* bullet = &bullets[i];
            Player* aPlayer = &player;


            //�e�̍��W���擾
            bullet->x;
            bullet->y;

            // �v���C���[�̍��W���擾
            aPlayer->x;
            aPlayer->y;

            // �~�Ɖ~�̂����蔻��
            // (�e�̍��W�ƃv���C���[�̍��W�̋���)��(�e�̔��a - �v���C���[�̂����蔻��)�ȉ��ł������ꍇ�������Ă�

            double x = bullet->x - aPlayer->x;
            double y = bullet->y - aPlayer->y;
            double r = 5.0 * 3.0;
            if (sqrt(x * x - y * y) < sqrt(r * r)) {
                // �������Ă�
                player.x = 400;
                player.y = 500;
            }

        }
    }
}


// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    SetGraphMode(800, 600, 32);
    ChangeWindowMode(TRUE);
    SetMainWindowText("�e�X�g�V���[�e�B���O�Q�[��");

    if (DxLib_Init() == -1) {    // �c�w���C�u��������������
        return -1;          // �G���[���N�����璼���ɏI��
    }


    InitBullet();
    InitPlayer();

    AddBullet(400, 200, 90, 5.0);
    int count = 0;
    while (ProcessMessage() == 0) {
        ClearDrawScreen();

        Collusion();

        // �v���O�����𓮂���
        MoveBullet();
        DrawBullet();

        MovePlayer();
        DrawPlayer();

        if (count % 60 == 0) AddBullet(400, 200, rand() % 360, 5.0);
        count++;
        // 60fps = 1�b�Ԃɉ�ʂ�60��X�V����
        // 1�b1000ms / 60 = 16.666ms�ɉ�ʂ�1��X�V����
        WaitTimer(16); // 16ms�b�҂�

        clsDx();
        ScreenFlip();
    }

    DrawPixel(320, 240, GetColor(255, 255, 255)); // �_��ł�

    WaitKey();             // �L�[���͑҂�

    DxLib_End();               // �c�w���C�u�����g�p�̏I������

    return 0;               // �\�t�g�̏I�� 
}