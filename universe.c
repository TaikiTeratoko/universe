#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>

#define NUM_METEORS 100// 流星の数

#ifndef M_PI
#define M_PI 3.14159265358979323846 // 定数 π を定義
#endif

// 年と日を表す変数
static double year = 0, sun_day = 0, mercury_day = 0, venus_day = 0, earth_day = 0, mars_day = 0, jupiter_day = 0, saturn_day = 0, uranus_day = 0, neptune_day = 0, moon_day = 0, pluto_day = 0;
// カメラの角度と距離を表す変数
static float camera_angle_x = 50.0f;
static float camera_angle_y = 35.0f;
static float camera_distance = 5.0f; // カメラの初期距離を太陽系の中心に近づける
static int last_mouse_x, last_mouse_y; // マウスの最後の位置を保存する変数

// 流星の情報を保持する構造体
typedef struct {
    double x, y, z; // 位置
    double vx, vy, vz; // 速度
    double speed; // 速度の大きさ
} Meteor;

Meteor meteors[NUM_METEORS]; // 流星の配列

// 初期化関数
void myInit(char* progname)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ダブルバッファと RGBA モードで初期化
    glutInitWindowSize(10000, 20000); // ウィンドウサイズを設定
    glutInitWindowPosition(0, 0); // ウィンドウ位置を設定
    glutCreateWindow(progname); // ウィンドウを作成
    glClearColor(0.0, 0.0, 0.0, 0.0); // 背景色を黒に設定

    // 乱数の初期化
    srand(time(NULL));

    // 流星の初期位置と速度を設定する
    for (int i = 0; i < NUM_METEORS; i++) {
        // 初期位置をランダムに設定（広い範囲でランダム）
        meteors[i].x = -100.0 + (rand() % 401); // x座標は-100から100の間でランダム
        meteors[i].y = -100.0 + (rand() % 401); // y座標は-100から100の間でランダム
        meteors[i].z = -100.0 - (rand() % 300); // z座標は-100から-400の間でランダム

        // 速度の大きさをランダムに設定
        meteors[i].speed = 0.5 + (rand() % 46) * 0.01; // 速度は0.05から0.5の間でランダム

        // 速度の方向をランダムに設定
        meteors[i].vx = -0.1 + (rand() % 21) * 0.01; // x方向の速度（-0.1から0.1）
        meteors[i].vy = -0.1 + (rand() % 21) * 0.01; // y方向の速度（-0.1から0.1）
        meteors[i].vz = 0.5 + (rand() % 11) * 0.01; // z方向の速度（0.5から0.6）
    }

}

// 流星の初期化
void initializeMeteor(int i) {
    meteors[i].x = -100.0 + (rand() % 401);
    meteors[i].y = -100.0 + (rand() % 401);
    meteors[i].z = -100.0 - (rand() % 300);

    // 速度の大きさをランダムに設定
    meteors[i].speed = 0.05 + (rand() % 46) * 0.01;
    // 速度の初期値を0.05として、最大0.5までランダムに設定
    // (rand() % 46)は0から45までの整数を生成
    // これを0.01倍することで0.00から0.45までの浮動小数点数を生成

    // 速度の方向をランダムに設定
    meteors[i].vx = -0.1 + (rand() % 21) * 0.01;
    // x方向の速度（-0.1から0.1までランダム）
    // (rand() % 21)は0から20までの整数を生成
    // これを0.01倍することで0.00から0.20までの浮動小数点数を生成
    // -0.1を加えることで、-0.1から0.1の範囲に変換

    meteors[i].vy = -0.1 + (rand() % 21) * 0.01;
    // y方向の速度（-0.1から0.1までランダム）
    // 上記のvxと同じ計算

    meteors[i].vz = 0.5 + (rand() % 11) * 0.01;
    // z方向の速度（0.5から0.6までランダム）
    // (rand() % 11)は0から10までの整数を生成
    // これを0.01倍することで0.00から0.10までの浮動小数点数を生成
    // 0.5を加えることで、0.5から0.6の範囲に変換
}

// 描画関数
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT); // カラーバッファをクリア
    glColor3d(1.0, 1.0, 1.0); // 描画色を白に設定

    glPushMatrix();
    // カメラの位置と回転を設定
    glTranslatef(0.0f, 0.0f, -camera_distance);
    glRotatef(camera_angle_y, 1.0, 0.0, 0.0);
    glRotatef(camera_angle_x, 0.0, 1.0, 0.0);

    // 太陽の描画
    glPushMatrix();
    double sun_rotation_period = 27.3; // 太陽の自転周期（地球日）
    glColor3d(1.0, 0.0, 0.0);// 太陽の色
    glRotated((double)sun_day / sun_rotation_period, 0.0, 1.0, 0.0); // 自転
    glutWireSphere(1.0, 20, 16); // 太陽の描画
    glPopMatrix();

    // 水星の描画
    glPushMatrix();
    double mercury_orbit_radius = 1.5; // 水星の軌道半径
    double mercury_eccentricity = 0.205; // 水星の離心率
    double mercury_orbit_period = 88.0; // 水星の公転周期（地球日）
    double mercury_rotation_period = 58.6; // 水星の自転周期（地球日）
    double mercury_theta = (double)year * 2.0 * M_PI / mercury_orbit_period; // 現在の角度
    double mercury_r = mercury_orbit_radius * (1 - mercury_eccentricity * mercury_eccentricity) /
        (1 + mercury_eccentricity * sin(mercury_theta)); // 楕円軌道の半径
    double mercury_x = mercury_r * sin(mercury_theta); // x座標
    double mercury_z = mercury_r * cos(mercury_theta); // z座標
    glRotated(7.0, 0.0, 0.0, 1.0); // 軌道面の傾き
    glTranslated(mercury_x, 0.0, mercury_z); // 軌道上の位置へ移動
    glRotated(2.11, 0.0, 0.0, 1.0); // 自転軸の傾き
    // 自転軸の視覚化
    glColor3d(1.0, 1.0, 1.0); // 自転軸の色
    glBegin(GL_LINES);
    glVertex3d(0.0, -0.08, 0.0);
    glVertex3d(0.0, 0.08, 0.0);
    glEnd();
    glColor3d(0.0, 0.75, 1.0);  // 水星の色
    glRotated((double)mercury_day / mercury_rotation_period, 0.0, 1.0, 0.0); // 自転
    glutWireSphere(0.05, 10, 8); // 水星の描画
    glPopMatrix();

    // 金星の描画
    glPushMatrix();
    double venus_orbit_radius = 2.5; // 金星の軌道半径（単位は天文単位）
    double venus_eccentricity = 0.0067; // 金星の離心率
    double venus_orbit_period = 224.7; // 金星の公転周期（地球日）
    double venus_rotation_period = -243.0; // 金星の自転周期（地球日、逆行）
    double venus_theta = (double)year * 2.0 * M_PI / venus_orbit_period; // 現在の角度
    double venus_r = venus_orbit_radius * (1 - venus_eccentricity * venus_eccentricity) /
        (1 + venus_eccentricity * sin(venus_theta)); // 楕円軌道の半径
    double venus_x = venus_r * sin(venus_theta); // x座標
    double venus_z = venus_r * cos(venus_theta); // z座標
    glRotated(3.39, 0.0, 0.0, 1.0); // 軌道面の傾き
    glTranslated(venus_x, 0.0, venus_z); // 軌道上の位置へ移動
    glRotated(177.4, 0.0, 0.0, 1.0); // 自転軸の傾き
    // 自転軸の視覚化
    glColor3d(1.0, 1.0, 1.0); // 自転軸の色
    glBegin(GL_LINES);
    glVertex3d(0.0, -0.15, 0.0);
    glVertex3d(0.0, 0.15, 0.0);
    glEnd();
    glColor3d(1.0, 1.0, 0.0); // 金星の色
    glRotated((double)venus_day / venus_rotation_period * 360.0, 0.0, 1.0, 0.0); // 自転
    glutWireSphere(0.1, 10, 8); // 金星の描画
    glPopMatrix();

    // 地球の描画
    glPushMatrix();
    double earth_orbit_radius = 3.5; // 地球の軌道半径（単位は天文単位）
    double earth_eccentricity = 0.0167; // 地球の離心率
    double earth_orbit_period = 365.25; // 地球の公転周期（地球日）
    double earth_rotation_period = 0.99; // 地球の自転周期（地球日）
    double earth_theta = (double)year * 2.0 * M_PI / earth_orbit_period; // 現在の角度
    double earth_r = earth_orbit_radius * (1 - earth_eccentricity * earth_eccentricity) /
        (1 + earth_eccentricity * sin(earth_theta)); // 楕円軌道の半径
    double earth_x = earth_r * sin(earth_theta); // x座標
    double earth_z = earth_r * cos(earth_theta); // z座標
    glRotated(0.0, 0.0, 0.0, 1.0); // 軌道面の傾き
    glTranslated(earth_x, 0.0, earth_z); // 軌道上の位置へ移動
    glRotated(23.44, 0.0, 0.0, 1.0); // 自転軸の傾き
    // 自転軸の視覚化
    glColor3d(1.0, 1.0, 1.0); // 自転軸の色
    glBegin(GL_LINES);
    glVertex3d(0.0, -0.25, 0.0);
    glVertex3d(0.0, 0.25, 0.0);
    glEnd();
    glColor3d(0.0, 0.0, 1.0); // 地球の色
    glRotated((double)earth_day / earth_rotation_period, 0.0, 1.0, 0.0); // 自転
    glutWireSphere(0.2, 10, 8); // 地球の描画
    //月
    glPushMatrix();
    glRotated((double)moon_day / 27.3, 0.0, 1.0, 0.0); //月の公転
    glTranslated(0.3, 0.0, 0.0); // 地球からの距離
    glRotated((double)moon_day / 27.3, 0.0, 1.0, 0.0); // 自転
    glColor3d(0.7, 0.7, 0.55); // グレー色 
    glutWireSphere(0.03, 10, 8); // 半径, 緯線, 経線

    glPopMatrix();
    glPopMatrix();

    // 火星の描画
    glPushMatrix();
    double mars_orbit_radius = 5.0; // 火星の軌道半径（単位は天文単位）
    double mars_eccentricity = 0.0934; // 火星の離心率
    double mars_orbit_period = 687.0; // 火星の公転周期（地球日）
    double mars_rotation_period = 1.03; // 火星の自転周期（地球日）
    double mars_theta = (double)year * 2.0 * M_PI / mars_orbit_period; // 現在の角度
    double mars_r = mars_orbit_radius * (1 - mars_eccentricity * mars_eccentricity) /
        (1 + mars_eccentricity * sin(mars_theta)); // 楕円軌道の半径
    double mars_x = mars_r * sin(mars_theta); // x座標
    double mars_z = mars_r * cos(mars_theta); // z座標
    glRotated(1.85, 0.0, 0.0, 1.0); // 軌道面の傾き
    glTranslated(mars_x, 0.0, mars_z); // 軌道上の位置へ移動
    glRotated(25.19, 0.0, 0.0, 1.0); // 自転軸の傾き
    // 自転軸の視覚化
    glColor3d(1.0, 1.0, 1.0); // 自転軸の色
    glBegin(GL_LINES);
    glVertex3d(0.0, -0.2, 0.0);
    glVertex3d(0.0, 0.2, 0.0);
    glEnd();
    glColor3d(1.0, 0.5, 0.0); // 火星の色
    glRotated((double)mars_day / mars_rotation_period, 0.0, 1.0, 0.0); // 自転
    glutWireSphere(0.15, 10, 8); // 火星の描画
    glPopMatrix();

    // 木星の描画
    glPushMatrix();
    double jupiter_orbit_radius = 10.0; // 木星の軌道半径（単位は天文単位）
    double jupiter_eccentricity = 0.048; // 木星の離心率
    double jupiter_orbit_period = 4331.0; // 木星の公転周期（地球日）
    double jupiter_rotation_period = 0.41; // 木星の自転周期（地球日）
    double jupiter_theta = (double)year * 2.0 * M_PI / jupiter_orbit_period; // 現在の角度
    double jupiter_r = jupiter_orbit_radius * (1 - jupiter_eccentricity * jupiter_eccentricity) /
        (1 + jupiter_eccentricity * sin(jupiter_theta)); // 楕円軌道の半径
    double jupiter_x = jupiter_r * sin(jupiter_theta); // x座標
    double jupiter_z = jupiter_r * cos(jupiter_theta); // z座標
    glRotated(1.3, 0.0, 0.0, 1.0); // 軌道面の傾き
    glTranslated(jupiter_x, 0.0, jupiter_z); // 軌道上の位置へ移動
    glRotated(3.13, 0.0, 0.0, 1.0); // 自転軸の傾き
    // 自転軸の視覚化
    glColor3d(1.0, 1.0, 1.0); // 自転軸の色
    glBegin(GL_LINES);
    glVertex3d(0.0, -0.45, 0.0);
    glVertex3d(0.0, 0.45, 0.0);
    glEnd();
    glColor3d(0.0, 1.0, 0.0); // 木星の色
    glRotated((double)jupiter_day / jupiter_rotation_period, 0.0, 1.0, 0.0); // 自転
    glutWireSphere(0.4, 10, 8); // 木星の描画
    glPopMatrix();

    // 土星の描画
    glPushMatrix();
    double saturn_orbit_radius = 13.0; // 土星の軌道半径（単位は天文単位）
    double saturn_eccentricity = 0.0565; // 土星の離心率
    double saturn_orbit_period = 10747.0; // 土星の公転周期（地球日）
    double saturn_rotation_period = 0.44; // 土星の自転周期（地球日）
    double saturn_theta = (double)year * 2.0 * M_PI / saturn_orbit_period; // 現在の角度
    double saturn_r = saturn_orbit_radius * (1 - saturn_eccentricity * saturn_eccentricity) /
        (1 + saturn_eccentricity * sin(saturn_theta)); // 楕円軌道の半径
    double saturn_x = saturn_r * sin(saturn_theta); // x座標
    double saturn_z = saturn_r * cos(saturn_theta); // z座標
    glRotated(2.48, 0.0, 0.0, 1.0); // 軌道面の傾き
    glTranslated(saturn_x, 0.0, saturn_z); // 軌道上の位置へ移動
    glRotated(26.73, 0.0, 0.0, 1.0); // 自転軸の傾き
    // 自転軸の視覚化
    glColor3d(1.0, 1.0, 1.0); // 自転軸の色
    glBegin(GL_LINES);
    glVertex3d(0.0, -0.4, 0.0);
    glVertex3d(0.0, 0.4, 0.0);
    glEnd();
    glColor3d(0.6, 0.3, 0.0); // 土星の色
    glRotated((double)saturn_day / saturn_rotation_period, 0.0, 1.0, 0.0); // 自転
    glutWireSphere(0.35, 10, 8); // 土星の描画
    // 土星のリングの描画
    glColor3d(0.8, 0.8, 0.8); // リングの色
    double inner_radius = 0.5; // リングの内側の半径
    double outer_radius = 0.8; // リングの外側の半径

    int num_segments = 100; // セグメントの数

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= num_segments; i++) {
        double theta = 2.0 * M_PI * (double)i / num_segments;
        double x_inner = cos(theta) * inner_radius;
        double z_inner = sin(theta) * inner_radius;
        double x_outer = cos(theta) * outer_radius;
        double z_outer = sin(theta) * outer_radius;

        glVertex3d(x_inner, 0.0, z_inner); // 内側の円の頂点
        glVertex3d(x_outer, 0.0, z_outer); // 外側の円の頂点
    }
    glEnd();

    glPopMatrix();

    // 天王星の描画
    glPushMatrix();
    double uranus_orbit_radius = 16.0; // 天王星の軌道半径（単位は天文単位）
    double uranus_eccentricity = 0.0457; // 天王星の離心率
    double uranus_orbit_period = 30589.0; // 天王星の公転周期（地球日）
    double uranus_rotation_period = -0.72; // 天王星の自転周期（地球日、逆行）
    double uranus_theta = (double)year * 2.0 * M_PI / uranus_orbit_period; // 現在の角度
    double uranus_r = uranus_orbit_radius * (1 - uranus_eccentricity * uranus_eccentricity) /
        (1 + uranus_eccentricity * sin(uranus_theta)); // 楕円軌道の半径
    double uranus_x = uranus_r * sin(uranus_theta); // x座標
    double uranus_z = uranus_r * cos(uranus_theta); // z座標
    glRotated(0.77, 0.0, 0.0, 1.0); // 軌道面の傾き
    glTranslated(uranus_x, 0.0, uranus_z); // 軌道上の位置へ移動
    glRotated(97.77, 0.0, 0.0, 1.0); // 自転軸の傾き
    // 自転軸の視覚化
    glColor3d(1.0, 1.0, 1.0); // 自転軸の色
    glBegin(GL_LINES);
    glVertex3d(0.0, -0.3, 0.0);
    glVertex3d(0.0, 0.3, 0.0);
    glEnd();
    glColor3d(0.5, 1.0, 1.0); // 天王星の色
    glRotated((double)uranus_day / uranus_rotation_period, 0.0, 1.0, 0.0); // 自転
    glutWireSphere(0.26, 10, 8); // 天王星の描画
    glPopMatrix();

    // 海王星の描画
    glPushMatrix();
    double neptune_orbit_radius = 19.0; // 海王星の軌道半径（単位は天文単位）
    double neptune_eccentricity = 0.0113; // 海王星の離心率
    double neptune_orbit_period = 59800.0; // 海王星の公転周期（地球日）
    double neptune_rotation_period = 0.67; // 海王星の自転周期（地球日）
    double neptune_theta = (double)year * 2.0 * M_PI / neptune_orbit_period; // 現在の角度
    double neptune_r = neptune_orbit_radius * (1 - neptune_eccentricity * neptune_eccentricity) /
        (1 + neptune_eccentricity * sin(neptune_theta)); // 楕円軌道の半径
    double neptune_x = neptune_r * sin(neptune_theta); // x座標
    double neptune_z = neptune_r * cos(neptune_theta); // z座標
    glRotated(1.77, 0.0, 0.0, 1.0); // 軌道面の傾き
    glTranslated(neptune_x, 0.0, neptune_z); // 軌道上の位置へ移動
    glRotated(28.32, 0.0, 0.0, 1.0); // 自転軸の傾き
    // 自転軸の視覚化
    glColor3d(1.0, 1.0, 1.0); // 自転軸の色
    glBegin(GL_LINES);
    glVertex3d(0.0, -0.3, 0.0);
    glVertex3d(0.0, 0.3, 0.0);
    glEnd();
    glColor3d(0.0, 0.8, 0.6); // 海王星の色
    glRotated((double)neptune_day / neptune_rotation_period, 0.0, 1.0, 0.0); // 自転
    glutWireSphere(0.25, 10, 8); // 海王星の描画
    glPopMatrix();

    // 冥王星の描画
    glPushMatrix();
    double pluto_orbit_radius = 24.5; // 冥王星の軌道半径（単位は天文単位）
    double pluto_eccentricity = 0.2488; // 冥王星の離心率
    double pluto_orbit_period = 90560.0; // 冥王星の公転周期（地球日）
    double pluto_rotation_period = 6.39; // 冥王星の自転周期（地球日）
    double pluto_theta = (double)year * 2.0 * M_PI / pluto_orbit_period; // 現在の角度
    double pluto_r = pluto_orbit_radius * (1 - pluto_eccentricity * pluto_eccentricity) /
        (1 + pluto_eccentricity * sin(pluto_theta)); // 楕円軌道の半径
    double pluto_x = pluto_r * sin(pluto_theta); // x座標
    double pluto_z = pluto_r * cos(pluto_theta); // z座標
    glRotated(17.16, 0.0, 0.0, 1.0); // 軌道面の傾き
    glTranslated(pluto_x, 0.0, pluto_z); // 軌道上の位置へ移動
    glRotated(122.5, 0.0, 0.0, 1.0); // 自転軸の傾き
    // 自転軸の視覚化
    glColor3d(1.0, 1.0, 1.0); // 自転軸の色
    glBegin(GL_LINES);
    glVertex3d(0.0, -0.25, 0.0);
    glVertex3d(0.0, 0.25, 0.0);
    glEnd();
    glColor3d(0.8, 0.65, 0.3); // 冥王星の色
    glRotated((double)pluto_day / pluto_rotation_period, 0.0, 1.0, 0.0); // 自転
    glutWireSphere(0.18, 10, 8); // 冥王星の描画
    glPopMatrix();


    // 各流星の描画と更新
    for (int i = 0; i < NUM_METEORS; i++) {
        glPushMatrix();
        glTranslated(meteors[i].x, meteors[i].y, meteors[i].z); // 流星の位置に移動
        glColor3d(1.0, 1.0, 1.0); // 流星の色を白に設定
        glutWireSphere(0.05, 10, 8); // 流星の描画
        glPopMatrix();

        // 流星の位置を更新
        meteors[i].x += meteors[i].vx * meteors[i].speed;
        meteors[i].y += meteors[i].vy * meteors[i].speed;
        meteors[i].z += meteors[i].vz * meteors[i].speed;

        // 流星が画面外に出たら、再び遠い位置から発生させる
        if (meteors[i].z > 0.0) {
            initializeMeteor(i);
        }
    }

    glPopMatrix();

    glutSwapBuffers(); // バッファを交換して描画を更新
}

// ウィンドウのリサイズ処理
void myReshape(int width, int height)
{
    // ビューポートの設定
    glViewport(0, 0, width, height);
    // 投影行列の設定
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // パースペクティブの設定
    gluPerspective(60.0, (double)width / (double)height, 0.1, 200.0);
    // モデルビュー行列の設定
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// マウスイベントに対応する関数
void myMouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) { // マウスボタンが押されたとき
        last_mouse_x = x; // マウスの x 座標を保存
        last_mouse_y = y; // マウスの y 座標を保存
    }
}

// マウスの移動に対応する関数
void myMotion(int x, int y)
{
    int dx = x - last_mouse_x; // x 座標の移動量
    int dy = y - last_mouse_y; // y 座標の移動量

    camera_angle_x += (float)dx * 0.1f; // カメラの x 方向の角度を更新
    camera_angle_y += (float)dy * 0.1f; // カメラの y 方向の角度を更新

    last_mouse_x = x; // マウスの x 座標を更新
    last_mouse_y = y; // マウスの y 座標を更新

    glutPostRedisplay(); // 再描画
}

// アニメーションの更新
void update(int value)
{
    glutPostRedisplay(); // 再描画を指示
    // アニメーションの更新を継続するためにタイマーを設定
    glutTimerFunc(16, update, 0); // 約60fpsで更新
}

// タイマー関数
void myTimer(int value)
{
    // 惑星の軌道を更新
    year = (year + 200);
    sun_day = (sun_day + 27.3);
    mercury_day =(mercury_day + 1.5);
    venus_day = (venus_day + 0.92);
    earth_day = (earth_day + 365.0);
    mars_day = (mars_day + 687.0);
    jupiter_day = (jupiter_day + 10563.0);
    saturn_day = (saturn_day + 24425.0);
    uranus_day = (uranus_day + 48734.0);
    neptune_day = (neptune_day + 89253.0);
    pluto_day = (pluto_day + 141415);
    // カメラの距離を更新
    if (camera_distance < 40.0f) {
        camera_distance += 0.02f;
    }

    // 再描画を指示
    glutPostRedisplay();
    // タイマーを再設定
    glutTimerFunc(16, myTimer, 0); // 約60FPSを目指すために16ミリ秒後に再度呼び出し
}

// 新しい流星を追加する関数
void addNewMeteor(int value)
{
    // まだ画面に出ていない流星を見つけて初期化する
    for (int i = 0; i < NUM_METEORS; i++) {
        if (meteors[i].z > 0.0) {
            initializeMeteor(i);
            break;
        }
    }

    // 新しい流星を一定間隔で追加するためにタイマーを設定
    glutTimerFunc(500, addNewMeteor, 0); // 0.5秒ごとに新しい流星を追加
}

// メイン関数
int main(int argc, char** argv)
{
    glutInit(&argc, argv); // GLUT を初期化
    myInit(argv[0]); // 初期化関数を呼び出し
    glutDisplayFunc(myDisplay); // 描画関数を設定
    glutReshapeFunc(myReshape); // リサイズイベント関数を設定
    glutMouseFunc(myMouse); // マウスイベント関数を設定
    glutMotionFunc(myMotion); // マウス移動イベント関数を設定
    glutTimerFunc(100, myTimer, 0); // タイマー関数を設定
    glutTimerFunc(16, update, 0); // 約60fpsで更新// アニメーションの開始
    glutTimerFunc(500, addNewMeteor, 0); // 0.5秒ごとに新しい流星を追加

    glutMainLoop(); // メインループを開始
    return 0;
}
