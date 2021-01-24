#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_BtnLogin_clicked();

    void on_BtnAlta_clicked();

    void on_TWAdminMuestraUsuarios_cellDoubleClicked(int row, int column);

    void on_BtnActualizarUsuario_clicked();

    void on_BtnBajaEliminarUsuario_clicked();

    void on_BtnBajaModificaUsuario_clicked();

    void on_BtnCargarUsr_2_clicked();

    void on_TBtnAltaFecha_2_clicked();

    void on_CalAlta_2_clicked(const QDate &date);

    void on_BtnCerrar_clicked();

    void on_BtnBajaSalir1_clicked();

    void on_BtnBorrarDatos_2_clicked();

    void on_BtnSalirAlta_2_clicked();

    void on_BtnAltaRep_clicked();

    void on_BtnRepoAgregarCargarLimpiarDatos_clicked();

    void on_BtnRepoAgregarCargarProducto_clicked();

    void on_BtnRepoRefreshProductos_clicked();

    void on_TWRepoConsultarProductos_cellDoubleClicked(int row, int column);

    void on_BtnRepoEliminarModificar_clicked();

    void on_BtnRepoEliminarEliminar_clicked();

    void on_BtnRepoModificarActualizar_2_clicked();

    void on_BtnRepoAgregarDirImagen_clicked();

    void on_BtnRepoModificarDirImagen_clicked();

    void on_BtnRepoEliminarVolver_clicked();

    void on_BtnModificarSalir1_clicked();

    void on_BtnSalirRep_clicked();

    void on_BtnSalirRep_2_clicked();

    void on_BtnVendAdherirProd_clicked();

    void on_LedVendPago_returnPressed();

    void on_BtnVendFinalizarCompra_clicked();

    void on_BtnVendPagar_clicked();

    void on_TBtnAltaFecha_3_clicked();

    void on_CalModifica_clicked(const QDate &date);

    void on_BtnInforme_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_CalAdminInformeDesde_clicked(const QDate &date);

    void on_CalAdminInformHasta_clicked(const QDate &date);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
