#include "gamescene.h"
#include "utils.h"
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QDir>
#include <QPainter>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
{
    onUserCreate();
}

GameScene::~GameScene()
{

}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_loopTime += m_deltaTime;
    while(m_loopTime > m_loopSpeed)
    {
        m_loopTime -= m_loopSpeed;
        handlePlayerInput();

        draw();
        resetStatus();
    }
}

void GameScene::onUserCreate()
{
    setBackgroundBrush(COLOR_STYLE::BACKGROUND);
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i] = new KeyStatus();
    }
    m_mouse = new MouseStatus();
    setSceneRect(0,0, SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(int(1000.0f/FPS));
    m_elapsedTimer.start();
}

void GameScene::renderScene()
{
    static int index = 0;
    QString fileName = QDir::currentPath() + QDir::separator() + "screen" + QString::number(index++) + ".png";
    QRect rect = sceneRect().toAlignedRect();
    QImage image(rect.size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    render(&painter);
    image.save(fileName);
    qDebug() << "saved " << fileName;
}

void GameScene::handlePlayerInput()
{
    if(m_keys[KEYBOARD::KeysMapper[Qt::Key_Z]]->m_released)
    {
        renderScene();//uncoment if want to make screenshots
    }
    if(m_keys[KEYBOARD::KeysMapper[Qt::Key_Space]]->m_released)
    {
        m_ticTacToe.reset();
    }
    else if(m_keys[KEYBOARD::KeysMapper[Qt::Key_D]]->m_released)
    {
    }
    else if(m_keys[KEYBOARD::KeysMapper[Qt::Key_S]]->m_released)
    {
    }
    else if(m_keys[KEYBOARD::KeysMapper[Qt::Key_W]]->m_released)
    {
    }
    if(m_mouse->m_released)
    {
        int x = m_mouse->m_x;
        int y = m_mouse->m_y;
        QPoint clickedPoint(x, y);
        m_ticTacToe.proccess(clickedPoint);
        emit currentPlayerChanged(m_ticTacToe.currentPlayer());
    }
}

void GameScene::resetStatus()
{
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i]->m_released = false;
    }
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i]->m_pressed = false;
    }
    m_mouse->m_released = false;
}

void GameScene::draw()
{
    clear();
    m_ticTacToe.drawBoard(*this);
    m_ticTacToe.drawObjects(*this);
    m_ticTacToe.drawWinner(*this);
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if(KEYBOARD::KeysMapper.contains(event->key()))
    {
        if(event->isAutoRepeat())
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = true;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
        }
        else
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = true;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held    = false;
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if(KEYBOARD::KeysMapper.contains(event->key()))
    {
        if(!event->isAutoRepeat())
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = false;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_released = true;
        }

    }
    QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    m_mouse->m_pressed = true;
    QGraphicsScene::mousePressEvent(event);
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    QGraphicsScene::mouseMoveEvent(event);
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    m_mouse->m_pressed = false;
    m_mouse->m_released = true;
    QGraphicsScene::mouseReleaseEvent(event);
}
