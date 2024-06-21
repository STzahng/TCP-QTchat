#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:

    void set_MenuBar();
    void set_ToolBar();
    void set_StatusBar();
    void set_central();
    //菜单栏
    void set_FileMenu();
    void set_EditMenu();
    void set_helpMenu();
    //工具栏
    void set_Font();
    void set_FontSize();
    void set_FontBlod();
    void set_FontItalic();
    void set_FontColor();
    //活动项
    void Open_File();
    void Save_File();
    void New_File();


    void FontCopy();
    void FontCut();
    void FontPaste();
    void FontResume();
    void FontCancel();

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QMenuBar*MenuBar;//菜单栏
    QMenu* FileMenu;//文件菜单
    QMenu* EditMenu;//编辑菜单
    QMenu* helpMenu;//帮助菜单
    QTextEdit* TextEdit;
    QToolBar* ToolBar;//工具栏

    QAction* open_Act;
    QAction* new_Act;
    QAction* save_Act;
    QAction *Copy_Act;
    QAction *Cut_Act;
    QAction *Paste_Act;
    QAction *Resume_Act;
    QAction *Cancel_Act;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
