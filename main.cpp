#include "DxLib.h"
#include "math.h"


class Bullet {
public:
    double x; // 弾の座標x
    double y; // 弾の座標y
    double vx; // 弾の移動量x
    double vy; // 弾の移動量y
    double angle; // 弾の角度
    double speed; // 弾の速度
    bool use; // 弾が使用されているか

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

            // 三角関数を利用し移動量を求める
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
            int Color = GetColor(0, 0, 255); // 青色を取得
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
    double x; // プレイヤーの座標x
    double y; // プレイヤーの座標y
    int image; // プレイヤーの画像
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

// プレイヤー動作のメソッド
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
        // 三平方の定理を利用して斜辺の値を計算
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

// あたり判定のメソッド
void Collusion(){
    // 弾の座標とプレイヤーの座標が同じであればOUT
    for (int i = 0; i < BulletMax; i++) {
        if (bullets[i].use) {
            Bullet* bullet = &bullets[i];
            Player* aPlayer = &player;


            //弾の座標を取得
            bullet->x;
            bullet->y;

            // プレイヤーの座標を取得
            aPlayer->x;
            aPlayer->y;

            // 円と円のあたり判定
            // (弾の座標とプレイヤーの座標の距離)が(弾の半径 - プレイヤーのあたり判定)以下であった場合あたってる

            double x = bullet->x - aPlayer->x;
            double y = bullet->y - aPlayer->y;
            double r = 5.0 * 3.0;
            if (sqrt(x * x - y * y) < sqrt(r * r)) {
                // あたってる
                player.x = 400;
                player.y = 500;
            }

        }
    }
}


// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    SetGraphMode(800, 600, 32);
    ChangeWindowMode(TRUE);
    SetMainWindowText("テストシューティングゲーム");

    if (DxLib_Init() == -1) {    // ＤＸライブラリ初期化処理
        return -1;          // エラーが起きたら直ちに終了
    }


    InitBullet();
    InitPlayer();

    AddBullet(400, 200, 90, 5.0);
    int count = 0;
    while (ProcessMessage() == 0) {
        ClearDrawScreen();

        Collusion();

        // プログラムを動かす
        MoveBullet();
        DrawBullet();

        MovePlayer();
        DrawPlayer();

        if (count % 60 == 0) AddBullet(400, 200, rand() % 360, 5.0);
        count++;
        // 60fps = 1秒間に画面を60回更新する
        // 1秒1000ms / 60 = 16.666msに画面を1回更新する
        WaitTimer(16); // 16ms秒待つ

        clsDx();
        ScreenFlip();
    }

    DrawPixel(320, 240, GetColor(255, 255, 255)); // 点を打つ

    WaitKey();             // キー入力待ち

    DxLib_End();               // ＤＸライブラリ使用の終了処理

    return 0;               // ソフトの終了 
}