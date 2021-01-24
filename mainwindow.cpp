#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QFile"
#include "unistd.h"
#include "QTextStream"
#include <QCloseEvent>
#include <QTableWidget>
#include "QFileDialog"
#include "QtPrintSupport/QPrinter"
#include "QtPrintSupport/QPrintDialog"

struct Usuarios {
    QString user;
    QString pass;
    QString nombre;
    QString apellido;
    QString categoria;
    QString fechaAlta;
};
struct Productos {
  unsigned int codigo;
  unsigned int cantidad;
  QString marca;
  QString producto;
  QString descripcion;
  QString imagen;
  float costo;
};

bool comprobarVacio(QFile &);
void tomarDatosBorrar(QFile &, QFile &, Usuarios, QString);
Usuarios tomaDatosRenglon(QFile &, Usuarios, QString);
bool ExisteCodigo(int);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(300,300,400,200);
    this->setMinimumWidth(400);
    this->setMinimumHeight(200);
    this->setMaximumWidth(400);
    this->setMaximumHeight(200);
    ui->WLoginInicio->show();
    ui->WAdministrador->hide();
    ui->WRepositor->hide();
    ui->WVendedor->hide();
    ui->WAltaUsuario->hide();
    ui->WBajaUsuario->hide();
    ui->WBaja1Usuario->hide();
    ui->WMuestraUsuariosAdmin->hide();
    ui->WModificaUsuario->hide();
    ui->WModifica1Usuario->hide();
    ui->WRepoConsultarProd->hide();
    ui->WRepositorStockAgregar->hide();
    ui->WRepositorStockEliminarProducto->hide();
    ui->WRepositorStockModficarProducto->hide();
    ui->WRepositorStockModificar->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
// FUNCIONES PROPIAS ***********************************************************************************************************************************************************
void tomarDatosBorrar(QFile &arch, QFile &arch1, Usuarios usr, QString prueba)
{
    QString renglon;
    unsigned int datosStr = 0;
    QTextStream out(&arch1);

    while(!arch.atEnd())
    {
        do{
            renglon = arch.readLine();
            renglon.chop(1);
            switch(datosStr)
            {
            case 0:
                usr.nombre = renglon;
                break;
            case 1:
                usr.apellido = renglon;
                break;
            case 2:
                usr.categoria = renglon;
                break;
            case 3:
                usr.pass = renglon;
                break;
            case 4:
                usr.user = renglon;
                break;
            case 5:
                usr.fechaAlta = renglon;
                break;
            default: ;
            }
            datosStr++;
        }while(!renglon.isEmpty());
        datosStr = 0;
        if(prueba.compare(usr.user) != 0 && usr.user.compare("")!=0)
        {
            out << "\n";
            out << usr.nombre << "\n";
            out << usr.apellido << "\n";
            out << usr.categoria << "\n";
            out << usr.pass << "\n";
            out << usr.user << "\n";
            out << usr.fechaAlta << "\n";

        }
    }
}
Usuarios tomaDatosRenglon(QFile &arch, Usuarios usr, QString prueba)
{
    QString renglon;
    unsigned int datosStr = 0;
    while(!arch.atEnd())
    {

        do{
            renglon = arch.readLine();
            //QMessageBox::about(NULL,"tomaDAtos", "** ("+usr.user+")("+renglon+") ");
            renglon.chop(1);
            switch(datosStr)
            {
            case 0:
                usr.nombre = renglon;
                 //QMessageBox::about(NULL,"tomaDAtos1", " ("+usr.user+")("+renglon+") ");
                break;
            case 1:
                usr.apellido = renglon;
                break;
            case 2:
                usr.categoria = renglon;
                break;
            case 3:
                usr.pass = renglon;
                break;
            case 4:
                usr.user = renglon;
                //QMessageBox::about(NULL,"tomaDAtos", " ("+usr.user+")("+renglon+") ");
                break;
            case 5:
                usr.fechaAlta = renglon;
                break;
            default: ;
            }
            datosStr++;
        }while(!renglon.isEmpty());
        datosStr = 0;
        if((prueba.compare(usr.user) == 0))
            return usr;
    }
    usr.nombre = "";
    usr.apellido = "";
    usr.categoria = "";
    usr.user  = "";
    usr.pass = "";
    usr.fechaAlta = "";
    return usr;
}
bool comprobarVacio(QFile &arch)
{
    QString prueba;
    prueba = arch.readLine();
    if(prueba.isEmpty())
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool ExisteCodigo(int codigo)
{
    QFile arch("arch/stock.txt");
    bool existe = false;
    if(arch.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        Productos prod;
        unsigned int contador = 0;
        QString renglon;
        while(!arch.atEnd())
        {
            do
            {
                renglon = arch.readLine();
                renglon.chop(1);
                switch (contador) {
                case 0:
                    prod.codigo = renglon.toInt();
                    break;
                case 1:
                    prod.cantidad = renglon.toInt();
                    break;
                case 2:
                    prod.marca = renglon;
                    break;
                case 3:
                    prod.producto = renglon;
                    break;
                case 4:
                    prod.descripcion = renglon;
                    break;
                case 5:
                    prod.costo = renglon.toFloat();
                    break;
                case 6:
                    prod.imagen = renglon;
                    break;
                default: ;
                    break;
                }
                contador++;
            }while(!renglon.isEmpty());
            if(codigo == prod.codigo)
            {
                existe = true;
            }
            contador = 0;
        }
    }
    else
    {
        QMessageBox::information(NULL, "Error", "No se ha podido abrir el archvo Stock.txt (FDU).");
    }
    arch.close();
    return existe;
}
// FIN FUNCIONES PROPIAS ************************************************************************************************************************************************

void MainWindow::on_BtnLogin_clicked()
{
    QString user = ui->LedUser->text();
    QString pass = ui->LedPass->text();
    QString archUsuarios = "arch/users.txt";
    user.trimmed().toLower();
    pass.trimmed();
    QFile usuarios(archUsuarios);
    Usuarios usr;
    if(usuarios.open(QIODevice::ReadOnly|QIODevice::Text)){
    if(!comprobarVacio(usuarios))
    {

        usr = tomaDatosRenglon(usuarios,usr, user); // Siempre colocar la asignacion de la estructura.
        //QMessageBox::information(NULL,"Hola",""+cantidadItems(usuarios));
    }
    }
    else
    {
     QMessageBox::about(NULL,"Error", "No se puede abrir el archivo.");
    }

    int intentos_admin = 3;
    int intentos_vend = 5;
    int intentos_repo = 5;
    int seg = 60;

    //QMessageBox::about(NULL,"hola", " *("+usr.user+")("+user+") ");

    if((user.compare(usr.user)==0)&&(usr.categoria.compare("Administrador") == 0))
   {
        if(intentos_admin > 0)
        {
        if(pass.compare(usr.pass)== 0)
        {
            ui->WLoginInicio->hide();
            this->setGeometry(200,100,800,600);
            ui->WAdministrador->setGeometry(0,0,800,600);
            this->setMinimumWidth(800);
            this->setMinimumHeight(600);
            this->setMaximumWidth(800);
            this->setMaximumHeight(600);
            ui->LblAdministrador->setText("Bienvenido Sr. Administrador "+usr.nombre+" "+usr.apellido+" que desea hacer: ");
            ui->WAdministrador->show();
            ui->WMuestraUsuariosAdmin->hide();
            ui->TWAdminMuestraUsuarios->show();
            ui->BtnAlta->show();
            ui->BtnInforme->setDisabled(false);

            ui->CalAdminInformeDesde->hide();
            ui->CalAdminInformHasta->hide();
            ui->LblAdminDesde->hide();
            ui->LedAdminFechaDesde->hide();
            ui->BtnAdminBuscarInforme->hide();
            ui->toolButton->hide();
            ui->LblAdminHasta->hide();
            ui->LedAdminFechaHasta->hide();
            ui->toolButton_2->hide();

            ui->TWAdminMuestraUsuarios->clearContents();
            ui->TWAdminMuestraUsuarios->setRowCount(0);
            QFile arch("arch/users.txt");
            if(arch.open(QIODevice::ReadOnly|QIODevice::Text))
            {
            QString renglon;

            unsigned int datosStr = 0;
            unsigned int fila = 0;

            Usuarios usr;
            while(!arch.atEnd())
            {
                do{
                    renglon = arch.readLine();
                    renglon.chop(1);

                    switch(datosStr)
                    {
                    case 0:
                        usr.nombre = renglon;
                        break;
                    case 1:
                        usr.apellido = renglon;
                        break;
                    case 2:
                        usr.categoria = renglon;
                        break;
                    case 3:
                        usr.pass = renglon;
                        break;
                    case 4:
                        usr.user = renglon;
                        break;
                    case 5:
                        usr.fechaAlta = renglon;
                        break;
                    default: ;
                    }
                    datosStr++;

                   }while(!renglon.isEmpty());
                if(usr.nombre.compare("") != 0 && usr.apellido.compare("") !=0)
                {

                    ui->TWAdminMuestraUsuarios->insertRow(fila);
                    QTableWidgetItem *nombre = new QTableWidgetItem(usr.nombre);
                    nombre->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *apellido = new QTableWidgetItem(usr.apellido);
                    apellido->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *categoria = new QTableWidgetItem(usr.categoria);
                    categoria->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *user = new QTableWidgetItem(usr.user);
                    user->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *pass = new QTableWidgetItem(usr.pass);
                    pass->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *fechaAlta = new QTableWidgetItem(usr.fechaAlta);
                    fechaAlta->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    ui->TWAdminMuestraUsuarios->setItem(fila,0,nombre);
                    ui->TWAdminMuestraUsuarios->setItem(fila,1,apellido);
                    ui->TWAdminMuestraUsuarios->setItem(fila,2,categoria);
                    ui->TWAdminMuestraUsuarios->setItem(fila,3,user);
                    ui->TWAdminMuestraUsuarios->setItem(fila,4,pass);
                    ui->TWAdminMuestraUsuarios->setItem(fila,5,fechaAlta);
                    fila++;
                }
                datosStr = 0;
            }
            ui->WMuestraUsuariosAdmin->show();
            }
            else
            {
                QMessageBox::information(NULL,"Error", "No se puede abrir el archivo.");
            }
            arch.close();

            ui->WAdministrador->show();
        }else
        {
            QMessageBox::about(NULL, "Contraseña Invalida.", "Le quedan X intentos.");
          //  QMessageBox::about(NULL, "Contraseña Invalida.", contarSeg(seg));
            ui->LedUser->setText(user);
            ui->LedPass->clear();
            ui->LedPass->setFocus();
        }
        }else
        {
            intentos_admin = 3;
        }
    }
   else if((user.compare(usr.user)==0)&&(usr.categoria.compare("Repositor") == 0))
    {
        if(intentos_repo > 0)
        {
        if(pass.compare(usr.pass)== 0)
        {
            ui->WLoginInicio->hide();
            this->setGeometry(200,100,800,600);
            ui->WRepositor->setGeometry(0,0,800,600);
            this->setMinimumWidth(800);
            this->setMinimumHeight(600);
            this->setMaximumWidth(800);
            this->setMaximumHeight(600);
            ui->LblRepositor->setText("Bienvenido Sr. "+usr.nombre+" "+usr.apellido);
            ui->lblCatRep->setText("Categoria: "+usr.categoria);
            ui->lblfechaInrep->setText("Fecha de Ingreso: "+usr.fechaAlta);

            ui->WRepositor->show();
            ui->WRepoConsultarProd->show();

            ui->TWRepoConsultarProductos->clearContents();
            ui->TWRepoConsultarProductos->setRowCount(0);

            QFile arch("arch/stock.txt");
            if(arch.open(QIODevice::ReadOnly|QIODevice::Text))
            {
            QString renglon;

            unsigned int datosStr = 0;
            unsigned int fila = 0;

            Productos prod;
            while(!arch.atEnd())
            {
                do{
                    renglon = arch.readLine();
                    renglon.chop(1);

                    switch(datosStr)
                    {
                    case 0:
                        prod.codigo = renglon.toInt();
                        break;
                    case 1:
                        prod.cantidad = renglon.toInt();
                        break;
                    case 2:
                        prod.marca = renglon;
                        break;
                    case 3:
                        prod.producto = renglon;
                        break;
                    case 4:
                        prod.descripcion = renglon;
                        break;
                    case 5:
                        prod.costo = renglon.toFloat();
                        break;
                    case 6:
                        prod.imagen = renglon;
                        break;
                    default: ;
                    }
                    datosStr++;

                   }while(!renglon.isEmpty());
                if(prod.codigo > 0)
                {
                    ui->TWRepoConsultarProductos->insertRow(fila);
                    QTableWidgetItem *codigo = new QTableWidgetItem(tr("%1").arg(prod.codigo));
                    codigo->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *cantidad = new QTableWidgetItem(tr("%1").arg(prod.cantidad));
                    cantidad->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *marca = new QTableWidgetItem(prod.marca);
                    marca->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *producto = new QTableWidgetItem(prod.producto);
                    producto->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *descripcion = new QTableWidgetItem(prod.descripcion);
                    descripcion->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *costo = new QTableWidgetItem(tr("%1").arg(prod.costo));
                    costo->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *imagen = new QTableWidgetItem(prod.imagen);
                    imagen->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    ui->TWRepoConsultarProductos->setItem(fila,0,codigo);
                    ui->TWRepoConsultarProductos->setItem(fila,1,cantidad);
                    ui->TWRepoConsultarProductos->setItem(fila,2,marca);
                    ui->TWRepoConsultarProductos->setItem(fila,3,producto);
                    ui->TWRepoConsultarProductos->setItem(fila,4,descripcion);
                    ui->TWRepoConsultarProductos->setItem(fila,5,costo);
                    ui->TWRepoConsultarProductos->setItem(fila,6,imagen);
                    fila++;
                }

                datosStr = 0;
            }
            ui->WMuestraUsuariosAdmin->show();
            }
            else
            {
                QMessageBox::information(NULL,"Error", "No se puede abrir el archivo.");
            }
            arch.close();
        }else
        {
            QMessageBox::about(NULL, "Contraseña Invalida.", "Le quedan X intentos.");
            ui->LedUser->setText(user);
            ui->LedPass->clear();
            ui->LedPass->setFocus();
        }
        }else
        {
           // intentos_repo = 5;
        }
   }
   else if((user.compare(usr.user)==0)&&(usr.categoria.compare("Vendedor") == 0))
   {
        if(intentos_vend > 0)
        {
        if(pass.compare(usr.pass) == 0)
        {
            ui->WLoginInicio->hide();
            this->setGeometry(200,100,800,600);
            ui->WVendedor->setGeometry(0,0,800,600);
            this->setMinimumWidth(800);
            this->setMinimumHeight(600);
            this->setMaximumWidth(800);
            this->setMaximumHeight(600);
            ui->LblVendedor->setText(""+usr.nombre+" "+usr.apellido);
            ui->lblCatVen->setText("Categoria: "+usr.categoria);
            ui->lblfechaInVend->setText("Fecha de Ingreso: "+usr.fechaAlta);
            ui->lblVendFecha->setText("Fecha: "+QDateTime::currentDateTimeUtc().toString("dd/MM/yyyy"));
            ui->lblVendHora->setText("Hora: "+QTime::currentTime().toString("hh:mm:ss"));
            ui->WVendedor->show();
            ui->TWVendProductosVenta->clearContents();
            ui->TWVendProductosVenta->setRowCount(0);
        }else
        {
            QMessageBox::about(NULL, "Contraseña Invalida.", "Le quedan X intentos.");
            ui->LedUser->setText(user);
            ui->LedPass->clear();
            ui->LedPass->setFocus();
        }
        }else
        {
           // intentos_vend = 5;
        }
    }
   else
   {
            QMessageBox::about(NULL, "Error", "Usuario Invalido, por favor coloque un usuario Valido.");
            ui->LedPass->clear();
            ui->LedUser->clear();
            ui->LedUser->setFocus();
    }
    usuarios.close();
}

void MainWindow::on_BtnAlta_clicked()
{
    ui->WAdministrador->hide();
    ui->WAltaUsuario->show();
    ui->WAltaUsuario->setGeometry(0,0,400,300);
    ui->CalAlta_2->hide();
    ui->LedAltaNombre_2->setFocus();

}

void MainWindow::on_TWAdminMuestraUsuarios_cellDoubleClicked(int row, int column)
{
    ui->WAdministrador->hide();
    this->setGeometry(100,100,400,300);
    this->setMinimumWidth(400);
    this->setMinimumHeight(300);
    this->setMaximumWidth(400);
    this->setMaximumHeight(300);
    ui->WBajaUsuario->setGeometry(0,0,400,300);
    ui->WBajaUsuario->show();
    ui->WBaja1Usuario->setGeometry(10, 10, 371, 381);
    ui->WBaja1Usuario->show();
    ui->lblBajaNombre2->setText(ui->TWAdminMuestraUsuarios->item(row,0)->text());
    ui->lblBajaApellido2->setText(ui->TWAdminMuestraUsuarios->item(row,1)->text());
    ui->lblBajaCategoria2->setText(ui->TWAdminMuestraUsuarios->item(row,2)->text());
    ui->lblBajaUsuario2->setText(ui->TWAdminMuestraUsuarios->item(row,3)->text());
    ui->lblBajaPassword2->setText(ui->TWAdminMuestraUsuarios->item(row,4)->text());
    ui->lblBajaFechIngreso2->setText(ui->TWAdminMuestraUsuarios->item(row,5)->text());
}

void MainWindow::on_BtnActualizarUsuario_clicked()
{
    QFile arch("arch/users.txt");
    QFile arch1("arch/users.tmp");
    Usuarios usr;
    if(arch.open(QIODevice::ReadOnly|QIODevice::Text) && arch1.open(QIODevice::WriteOnly|QIODevice::Text))
    {
    QString renglon;
    QString user = ui->lblBajaUsuario2->text();
    unsigned int datosStr = 0;
    QTextStream out(&arch1);

    while(!arch.atEnd())
    {
        do{
            renglon = arch.readLine();
            renglon.chop(1);
            switch(datosStr)
            {
            case 0:
                usr.nombre = renglon;
                break;
            case 1:
                usr.apellido = renglon;
                break;
            case 2:
                usr.categoria = renglon;
                break;
            case 3:
                usr.pass = renglon;
                break;
            case 4:
                usr.user = renglon;
                break;
            case 5:
                usr.fechaAlta = renglon;
                break;
            default: ;
            }
            datosStr++;
        }while(!renglon.isEmpty());
        datosStr = 0;
        if(user.compare(usr.user) == 0 && usr.user.compare("")!=0)
        {
            out << "\n";
            out << ui->LedModificaNombre->text() << "\n";
            out << ui->LedModificaApellido->text() << "\n";
            if(ui->RBmodificaCatRep->isChecked() == true)
            {
            out << "Repositor" << "\n";
            }
            else
            {
            out << "Vendedor" << "\n";
            }
            out << ui->LedModificaPassword->text() << "\n";
            out << ui->LedModificaUsuario_2->text() << "\n";
            out << ui->LedModificaFechIngreso->text() << "\n";

        }
        else if(user.compare(usr.user) != 0 && usr.user.compare("")!=0)
        {
            out << "\n";
            out << usr.nombre << "\n";
            out << usr.apellido << "\n";
            out << usr.categoria << "\n";
            out << usr.pass << "\n";
            out << usr.user << "\n";
            out << usr.fechaAlta << "\n";
        }
    }
    }
    else
    {
        QMessageBox::information(NULL,"Error", "No se puede abrir el archivo.");
    }
    arch.close();
    arch1.close();
    QMessageBox quitMessageBox;
    quitMessageBox.setWindowTitle("Actualizar");
    quitMessageBox.setIcon(QMessageBox::Question);
    quitMessageBox.setText("Realmente desea Actualizar al Usuario "+ui->lblBajaUsuario2->text()+"?");
    quitMessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    quitMessageBox.setDefaultButton(QMessageBox::No);
    quitMessageBox.button(QMessageBox::Yes)->setText("Sí");
    quitMessageBox.button(QMessageBox::No)->setText("No");
    if(quitMessageBox.exec() == QMessageBox::Yes )
    {
        arch.remove();
        arch1.rename("arch/users.txt");
        ui->WModificaUsuario->hide();
        ui->WAdministrador->show();
        ui->TWAdminMuestraUsuarios->clearContents();
        ui->TWAdminMuestraUsuarios->setRowCount(0);
        QFile arch("arch/users.txt");
        if(arch.open(QIODevice::ReadOnly|QIODevice::Text))
        {
        QString renglon;

        unsigned int datosStr = 0;
        unsigned int fila = 0;

        Usuarios usr;
        while(!arch.atEnd())
        {
            do{
                renglon = arch.readLine();
                renglon.chop(1);

                switch(datosStr)
                {
                case 0:
                    usr.nombre = renglon;
                    break;
                case 1:
                    usr.apellido = renglon;
                    break;
                case 2:
                    usr.categoria = renglon;
                    break;
                case 3:
                    usr.pass = renglon;
                    break;
                case 4:
                    usr.user = renglon;
                    break;
                case 5:
                    usr.fechaAlta = renglon;
                    break;
                default: ;
                }
                datosStr++;

               }while(!renglon.isEmpty());
            if(usr.nombre.compare("") != 0 && usr.apellido.compare("") !=0)
            {

                ui->TWAdminMuestraUsuarios->insertRow(fila);
                QTableWidgetItem *nombre = new QTableWidgetItem(usr.nombre);
                nombre->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *apellido = new QTableWidgetItem(usr.apellido);
                apellido->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *categoria = new QTableWidgetItem(usr.categoria);
                categoria->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *user = new QTableWidgetItem(usr.user);
                user->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *pass = new QTableWidgetItem(usr.pass);
                pass->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *fechaAlta = new QTableWidgetItem(usr.fechaAlta);
                fechaAlta->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                ui->TWAdminMuestraUsuarios->setItem(fila,0,nombre);
                ui->TWAdminMuestraUsuarios->setItem(fila,1,apellido);
                ui->TWAdminMuestraUsuarios->setItem(fila,2,categoria);
                ui->TWAdminMuestraUsuarios->setItem(fila,3,user);
                ui->TWAdminMuestraUsuarios->setItem(fila,4,pass);
                ui->TWAdminMuestraUsuarios->setItem(fila,5,fechaAlta);
                fila++;
            }

            datosStr = 0;
        }
        ui->WMuestraUsuariosAdmin->show();
        this->setGeometry(200,100,800,600);
        this->setMinimumWidth(800);
        this->setMinimumHeight(600);
        this->setMaximumWidth(800);
        this->setMaximumHeight(600);
        }
        else
        {
            QMessageBox::information(NULL,"Error", "No se puede abrir el archivo.");
        }
        arch.close();
    }
    else
    {
        arch1.remove();
    }
}

void MainWindow::on_BtnBajaEliminarUsuario_clicked()
{
    Usuarios usrTmp;
    QString user = ui->lblBajaUsuario2->text();
    QFile arch("arch/users.txt");
    QFile arch1("arch/users.tmp");
    if(arch.open(QIODevice::ReadOnly|QIODevice::Text) && arch1.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        tomarDatosBorrar(arch, arch1, usrTmp,user);
    }
    else
    {
        QMessageBox::information(NULL, "Error.", "No se puede abrir el archivo.");
    }
    usrTmp = tomaDatosRenglon(arch,usrTmp,user);
    arch.close();
    arch1.close();

    QMessageBox quitMessageBox;
    quitMessageBox.setWindowTitle("Eliminar");
    //quitMessageBox.setWindowIcon(QIcon(":/new/img/Salir.gif"));
    quitMessageBox.setIcon(QMessageBox::Question);
    quitMessageBox.setText("Realmente desea Eliminar al Usuario"+usrTmp.user+"?");
    quitMessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    quitMessageBox.setDefaultButton(QMessageBox::No);
    quitMessageBox.button(QMessageBox::Yes)->setText("Sí");
    quitMessageBox.button(QMessageBox::No)->setText("No");
    if(quitMessageBox.exec() == QMessageBox::Yes )
    {
        arch.remove();
        arch1.rename("arch/users.txt");
        ui->TWAdminMuestraUsuarios->clearContents();
        ui->TWAdminMuestraUsuarios->setRowCount(0);
        QFile arch("arch/users.txt");
        if(arch.open(QIODevice::ReadOnly|QIODevice::Text))
        {
        QString renglon;

        unsigned int datosStr = 0;
        unsigned int fila = 0;

        Usuarios usr;
        while(!arch.atEnd())
        {
            do{
                renglon = arch.readLine();
                renglon.chop(1);

                switch(datosStr)
                {
                case 0:
                    usr.nombre = renglon;
                    break;
                case 1:
                    usr.apellido = renglon;
                    break;
                case 2:
                    usr.categoria = renglon;
                    break;
                case 3:
                    usr.pass = renglon;
                    break;
                case 4:
                    usr.user = renglon;
                    break;
                case 5:
                    usr.fechaAlta = renglon;
                    break;
                default: ;
                }
                datosStr++;

               }while(!renglon.isEmpty());
            if(usr.nombre.compare("") != 0 && usr.apellido.compare("") !=0)
            {

                ui->TWAdminMuestraUsuarios->insertRow(fila);
                QTableWidgetItem *nombre = new QTableWidgetItem(usr.nombre);
                nombre->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *apellido = new QTableWidgetItem(usr.apellido);
                apellido->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *categoria = new QTableWidgetItem(usr.categoria);
                categoria->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *user = new QTableWidgetItem(usr.user);
                user->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *pass = new QTableWidgetItem(usr.pass);
                pass->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *fechaAlta = new QTableWidgetItem(usr.fechaAlta);
                fechaAlta->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                ui->TWAdminMuestraUsuarios->setItem(fila,0,nombre);
                ui->TWAdminMuestraUsuarios->setItem(fila,1,apellido);
                ui->TWAdminMuestraUsuarios->setItem(fila,2,categoria);
                ui->TWAdminMuestraUsuarios->setItem(fila,3,user);
                ui->TWAdminMuestraUsuarios->setItem(fila,4,pass);
                ui->TWAdminMuestraUsuarios->setItem(fila,5,fechaAlta);
                fila++;
            }

            datosStr = 0;
        }
        ui->WMuestraUsuariosAdmin->show();
        this->setGeometry(200,100,800,600);
        this->setMinimumWidth(800);
        this->setMinimumHeight(600);
        this->setMaximumWidth(800);
        this->setMaximumHeight(600);
        }
        else
        {
            QMessageBox::information(NULL,"Error", "No se puede abrir el archivo.");
        }
        arch.close();
    }
    else
    {
        arch1.remove();
    }

    ui->WBaja1Usuario->hide();
    ui->WBajaUsuario->hide();
    ui->lblBajaNombre2->clear();
    ui->lblBajaApellido2->clear();
    ui->lblBajaCategoria2->clear();
    ui->lblBajaUsuario2->clear();
    ui->lblBajaFechIngreso2->clear();
    ui->WAdministrador->show();
}

void MainWindow::on_BtnBajaModificaUsuario_clicked()
{
    this->setGeometry(100,100,400,300);
    this->setMinimumWidth(400);
    this->setMinimumHeight(300);
    this->setMaximumWidth(400);
    this->setMaximumHeight(300);
    ui->WBajaUsuario->hide();
    ui->CalModifica->hide();
    ui->WModificaUsuario->setGeometry(0,0,400,300);
    ui->WModifica1Usuario->setGeometry(10, 10,381, 181);
    ui->WModificaUsuario->show();
    ui->WModifica1Usuario->show();
    ui->LedModificaNombre->setText(ui->lblBajaNombre2->text());
    ui->LedModificaApellido->setText(ui->lblBajaApellido2->text());
    if(ui->lblBajaCategoria2->text().compare("Repositor") == 0)
    {
        ui->RBmodificaCatRep->setChecked(true);
    }
    else
    {
        ui->RBModificaVend->setChecked(true);
    }
    ui->LedModificaUsuario_2->setText(ui->lblBajaUsuario2->text());
    ui->LedModificaPassword->setText(ui->lblBajaPassword2->text());
    ui->LedModificaFechIngreso->setText(ui->lblBajaFechIngreso2->text());
}

void MainWindow::on_BtnCargarUsr_2_clicked()
{
    if(ui->LedAltaNombre_2->text() != "")
    {
        if(ui->LedAltaApellido_2->text() != "")
        {
            if(ui->LedAltaUser_2->text() != "")
            {
                if(ui->RBtnRepositor_2->isChecked() == true || ui->RBtnVendedor_2->isChecked() == true)
                {
                    if(ui->LedAltaPass_2->text() != "")
                    {
                        if(ui->LedAltaFechIngreso_2->text() != "")
                        {
                            QFile arch("arch/users.txt");
                            if(arch.open(QIODevice::Append|QIODevice::Text))
                            {

                                QString nombre, apellido, categoria, user, pass, fecha;
                                if(ui->RBtnRepositor_2->isChecked() == true)
                                {
                                    categoria = "Repositor";
                                 }
                                else
                                {
                                    categoria = "Vendedor";
                                 }
                                 nombre = ui->LedAltaNombre_2->text();
                                 apellido = ui->LedAltaApellido_2->text();
                                 user = ui->LedAltaUser_2->text();
                                 pass = ui->LedAltaPass_2->text();
                                 fecha = ui->LedAltaFechIngreso_2->text();

                                 QTextStream out(&arch);
                                 out << "\n";
                                 out << nombre << "\n";
                                 out << apellido << "\n";
                                 out << categoria << "\n";
                                 out << pass << "\n";
                                 out << user << "\n";
                                 out << fecha << "\n";
                                 arch.close();

                                 QMessageBox::information(NULL, "GRABACION EXITOSA", "El Empleado "+nombre+" "+apellido+" ha sido agregado correctamente.");
                                 ui->LedAltaApellido_2->clear();
                                 ui->LedAltaNombre_2->clear();
                                 ui->LedAltaUser_2->clear();
                                 ui->LedAltaPass_2->clear();
                                 ui->LedAltaFechIngreso_2->clear();
                                 ui->RBtnRepositor_2->clearFocus();

                                 ui->WAltaUsuario->hide();
                                 this->setGeometry(200,100,800,600);
                                 ui->WAdministrador->setGeometry(0,0,800,600);
                                 ui->WAdministrador->show();

                                 ui->TWAdminMuestraUsuarios->clearContents();
                                 ui->TWAdminMuestraUsuarios->setRowCount(0);
                                 QFile arch("arch/users.txt");
                                 if(arch.open(QIODevice::ReadOnly|QIODevice::Text))
                                 {
                                 QString renglon;

                                 unsigned int datosStr = 0;
                                 unsigned int fila = 0;

                                 Usuarios usr;
                                 while(!arch.atEnd())
                                 {
                                     do{
                                         renglon = arch.readLine();
                                         renglon.chop(1);

                                         switch(datosStr)
                                         {
                                         case 0:
                                             usr.nombre = renglon;
                                             break;
                                         case 1:
                                             usr.apellido = renglon;
                                             break;
                                         case 2:
                                             usr.categoria = renglon;
                                             break;
                                         case 3:
                                             usr.pass = renglon;
                                             break;
                                         case 4:
                                             usr.user = renglon;
                                             break;
                                         case 5:
                                             usr.fechaAlta = renglon;
                                             break;
                                         default: ;
                                         }
                                         datosStr++;

                                        }while(!renglon.isEmpty());
                                     if(usr.nombre.compare("") != 0 && usr.apellido.compare("") !=0)
                                     {

                                         ui->TWAdminMuestraUsuarios->insertRow(fila);
                                         QTableWidgetItem *nombre = new QTableWidgetItem(usr.nombre);
                                         nombre->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         QTableWidgetItem *apellido = new QTableWidgetItem(usr.apellido);
                                         apellido->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         QTableWidgetItem *categoria = new QTableWidgetItem(usr.categoria);
                                         categoria->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         QTableWidgetItem *user = new QTableWidgetItem(usr.user);
                                         user->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         QTableWidgetItem *pass = new QTableWidgetItem(usr.pass);
                                         pass->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         QTableWidgetItem *fechaAlta = new QTableWidgetItem(usr.fechaAlta);
                                         fechaAlta->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         ui->TWAdminMuestraUsuarios->setItem(fila,0,nombre);
                                         ui->TWAdminMuestraUsuarios->setItem(fila,1,apellido);
                                         ui->TWAdminMuestraUsuarios->setItem(fila,2,categoria);
                                         ui->TWAdminMuestraUsuarios->setItem(fila,3,user);
                                         ui->TWAdminMuestraUsuarios->setItem(fila,4,pass);
                                         ui->TWAdminMuestraUsuarios->setItem(fila,5,fechaAlta);
                                         fila++;
                                     }

                                     datosStr = 0;
                                 }
                                 ui->WMuestraUsuariosAdmin->show();
                                 this->setGeometry(200,100,800,600);
                                 this->setMinimumWidth(800);
                                 this->setMinimumHeight(600);
                                 this->setMaximumWidth(800);
                                 this->setMaximumHeight(600);
                                 }
                                 else
                                 {
                                     QMessageBox::information(NULL,"Error", "No se puede abrir el archivo.");
                                 }
                                 arch.close();

                            }
                            else
                            {
                                QMessageBox::about(NULL, "Error", "No se ha podido abrir el archivo.");
                            }
                        }
                        else
                        {
                            QMessageBox::about(NULL, "Error", "Falta la fecha de Ingreso.");
                            ui->LedAltaFechIngreso_2->setFocus();
                        }
                    }
                    else
                    {
                        QMessageBox::about(NULL, "Error", "Falta el Password.");
                        ui->LedAltaPass_2->setFocus();
                    }
                }
                else
                {
                    QMessageBox::about(NULL, "Error", "Falta la categoria.");
                    ui->RBtnRepositor_2->setFocus();
                }
            }
            else
            {
                QMessageBox::about(NULL, "Error", "Falta el Nombre de Usuario.");
                ui->LedAltaUser_2->setFocus();
            }
        }
        else
        {
            QMessageBox::about(NULL, "Error", "Falta el Apellido.");
            ui->LedAltaApellido_2->setFocus();
        }
    }
    else
    {
        QMessageBox::about(NULL, "Error", "Falta el nombre");
        ui->LedAltaNombre_2->setFocus();
    }
}

void MainWindow::on_TBtnAltaFecha_2_clicked()
{
    ui->CalAlta_2->show();
}

void MainWindow::on_CalAlta_2_clicked(const QDate &date)
{

    ui->LedAltaFechIngreso_2->setText(date.toString("dd-MM-yyyy"));
    ui->CalAlta_2->hide();
}

void MainWindow::on_BtnCerrar_clicked()
{
    ui->WAdministrador->hide();
    this->setGeometry(300,300,400,200);
    this->setMinimumWidth(400);
    this->setMinimumHeight(200);
    this->setMaximumWidth(400);
    this->setMaximumHeight(200);
    ui->WLoginInicio->show();
    ui->LedPass->clear();
    ui->LedUser->clear();
    ui->LedUser->setFocus();
}

void MainWindow::on_BtnBajaSalir1_clicked()
{
    ui->WAdministrador->show();
    ui->WMuestraUsuariosAdmin->show();
    ui->WBajaUsuario->hide();
    ui->WBaja1Usuario->hide();
    this->setGeometry(200,100,800,600);
    this->setMinimumWidth(800);
    this->setMinimumHeight(600);
    this->setMaximumWidth(800);
    this->setMaximumHeight(600);
}

void MainWindow::on_BtnBorrarDatos_2_clicked()
{
    ui->LedAltaNombre_2->clear();
    ui->LedAltaApellido_2->clear();
    ui->LedAltaUser_2->clear();
    ui->LedAltaPass_2->clear();
    ui->LedAltaFechIngreso_2->clear();
}

void MainWindow::on_BtnSalirAlta_2_clicked()
{
    ui->WAdministrador->show();
    ui->WMuestraUsuariosAdmin->show();
    ui->WAltaUsuario->hide();
    this->setGeometry(200,100,800,600);
    this->setMinimumWidth(800);
    this->setMinimumHeight(600);
    this->setMaximumWidth(800);
    this->setMaximumHeight(600);
}

void MainWindow::on_BtnAltaRep_clicked()
{
    ui->WRepositor->hide();
    this->setGeometry(200,300,760, 280);
    this->setMinimumWidth(760);
    this->setMinimumHeight(280);
    this->setMaximumWidth(760);
    this->setMaximumHeight(280);
    ui->WAdministrador->hide();
    ui->WRepositorStockAgregar->setGeometry(0,0,760,280);
    ui->WRepositorStockAgregar->show();
}

void MainWindow::on_BtnRepoAgregarCargarLimpiarDatos_clicked()
{
    ui->LedRepoAgregarCantidad->clear();
    ui->LedRepoAgregarMarca->clear();
    ui->LedRepoAgregarProducto->clear();
    ui->LedRepoAgregarDescripcion->clear();
    ui->LedRepoAgregarPrecioCosto->clear();
    ui->LedRepoAgregarImagen->clear();
    ui->LedRepoAgregarCantidad->setFocus();
}

void MainWindow::on_BtnRepoAgregarCargarProducto_clicked()
{
    if(ui->LedRepoAgregarCodigo->text() != "")
    {
        if(ui->LedRepoAgregarCantidad->text() != "")
        {
            if(ui->LedRepoAgregarMarca->text() != "")
            {
                if(ui->LedRepoAgregarProducto->text() != "")
                {
                    if(ui->LedRepoAgregarDescripcion->text() != "")
                    {
                        if(ui->LedRepoAgregarPrecioCosto->text() != "")
                        {
                        if(ui->LedRepoAgregarImagen->text() != "")
                        {
                            QFile arch("arch/stock.txt");
                            if(arch.open(QIODevice::Append|QIODevice::Text))
                            {

                                 QString producto, descripcion, imagen, marca;
                                 unsigned int codigo, cantidad;
                                 float costo;
                                 codigo = ui->LedRepoAgregarCodigo->text().toInt();
                                 cantidad = ui->LedRepoAgregarCantidad->text().toInt();
                                 marca = ui->LedRepoAgregarMarca->text();
                                 producto = ui->LedRepoAgregarProducto->text();
                                 descripcion = ui->LedRepoAgregarDescripcion->text();
                                 costo = ui->LedRepoAgregarPrecioCosto->text().toFloat();
                                 imagen = ui->LedRepoAgregarImagen->text();

                                 QTextStream out(&arch);
                                 out << "\n";
                                 out << codigo << "\n";
                                 out << cantidad << "\n";
                                 out << marca << "\n";
                                 out << producto << "\n";
                                 out << descripcion << "\n";
                                 out << costo << "\n";
                                 out << imagen << "\n";
                                 arch.close();

                                 QMessageBox::information(NULL, "GRABACION EXITOSA", "El Producto "+producto+" ha sido agregado correctamente.");
                                 ui->LedRepoAgregarCodigo->clear();
                                 ui->LedRepoAgregarCantidad->clear();
                                 ui->LedRepoAgregarMarca->clear();
                                 ui->LedRepoAgregarProducto->clear();
                                 ui->LedRepoAgregarDescripcion->clear();
                                 ui->LedRepoAgregarPrecioCosto->clear();
                                 ui->LedRepoAgregarImagen->clear();

                                 ui->WRepositorStockAgregar->hide();
                                 ui->WRepositor->show();

                                 ui->TWRepoConsultarProductos->clearContents();
                                 ui->TWRepoConsultarProductos->setRowCount(0);
                                 QFile arch("arch/stock.txt");
                                 if(arch.open(QIODevice::ReadOnly|QIODevice::Text))
                                 {
                                 QString renglon;

                                 unsigned int datosStr = 0;
                                 unsigned int fila = 0;

                                 Productos prod;
                                 while(!arch.atEnd())
                                 {
                                     do{
                                         renglon = arch.readLine();
                                         renglon.chop(1);

                                         switch(datosStr)
                                         {
                                         case 0:
                                             prod.codigo = renglon.toInt();
                                             break;
                                         case 1:
                                             prod.cantidad = renglon.toInt();
                                             break;
                                         case 2:
                                             prod.marca = renglon;
                                             break;
                                         case 3:
                                             prod.producto = renglon;
                                             break;
                                         case 4:
                                             prod.descripcion = renglon;
                                             break;
                                         case 5:
                                             prod.costo = renglon.toFloat();
                                             break;
                                         case 6:
                                             prod.imagen = renglon;
                                             break;
                                         default: ;
                                         }
                                         datosStr++;

                                        }while(!renglon.isEmpty());
                                     if(prod.codigo > 0)
                                     {
                                         ui->TWRepoConsultarProductos->insertRow(fila);
                                         QTableWidgetItem *codigo = new QTableWidgetItem(tr("%1").arg(prod.codigo));
                                         codigo->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         QTableWidgetItem *cantidad = new QTableWidgetItem(tr("%1").arg(prod.cantidad));
                                         cantidad->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         QTableWidgetItem *marca = new QTableWidgetItem(prod.marca);
                                         marca->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         QTableWidgetItem *producto = new QTableWidgetItem(prod.producto);
                                         producto->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         QTableWidgetItem *descripcion = new QTableWidgetItem(prod.descripcion);
                                         descripcion->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         QTableWidgetItem *costo = new QTableWidgetItem(tr("%1").arg(prod.costo));
                                         costo->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         QTableWidgetItem *imagen = new QTableWidgetItem(prod.imagen);
                                         imagen->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                                         ui->TWRepoConsultarProductos->setItem(fila,0,codigo);
                                         ui->TWRepoConsultarProductos->setItem(fila,1,cantidad);
                                         ui->TWRepoConsultarProductos->setItem(fila,2,marca);
                                         ui->TWRepoConsultarProductos->setItem(fila,3,producto);
                                         ui->TWRepoConsultarProductos->setItem(fila,4,descripcion);
                                         ui->TWRepoConsultarProductos->setItem(fila,5,costo);
                                         ui->TWRepoConsultarProductos->setItem(fila,6,imagen);
                                         fila++;
                                     }

                                     datosStr = 0;
                                 }
                                 ui->WRepoConsultarProd->show();
                                 this->setGeometry(200,100,800,600);
                                 this->setMinimumWidth(800);
                                 this->setMinimumHeight(600);
                                 this->setMaximumWidth(800);
                                 this->setMaximumHeight(600);
                                 }
                                 else
                                 {
                                     QMessageBox::information(NULL,"Error", "No se puede abrir el archivo.");
                                 }
                                 arch.close();

                            }
                            else
                            {
                                QMessageBox::about(NULL, "Error", "No se ha podido abrir el archivo.");
                            }
                        }
                        else
                        {
                            QMessageBox::about(NULL, "Error", "Falta la Imagen del Producto.");
                            ui->LedRepoAgregarImagen->setFocus();
                        }
                        }
                        else
                        {
                            QMessageBox::about(NULL, "Error", "Falta el costo del Producto.");
                            ui->LedRepoAgregarImagen->setFocus();
                        }
                    }
                    else
                    {
                        QMessageBox::about(NULL, "Error", "Falta la Descripcion del Producto.");
                        ui->LedRepoAgregarDescripcion->setFocus();
                    }
                }
                else
                {
                    QMessageBox::about(NULL, "Error", "Falta la Marca del Producto.");
                    ui->LedRepoAgregarMarca->setFocus();
                }

            }
            else
            {
                QMessageBox::about(NULL, "Error", "Falta el Nombre del Producto.");
                ui->LedRepoAgregarProducto->setFocus();
            }
        }
        else
        {
            QMessageBox::about(NULL, "Error", "La cantidad no puede ser menor o igual a 0.");
            ui->LedRepoAgregarCantidad->setFocus();
        }
    }
    else
    {
        QMessageBox::about(NULL, "Error", "El código debe ser mayor que 0");
        ui->LedRepoAgregarCodigo->setFocus();
    }
}

void MainWindow::on_BtnRepoRefreshProductos_clicked()
{

}

void MainWindow::on_TWRepoConsultarProductos_cellDoubleClicked(int row, int column)
{
    ui->WRepositor->hide();
    this->setGeometry(200,200,760, 280);
    this->setMinimumWidth(760);
    this->setMinimumHeight(280);
    this->setMaximumWidth(760);
    this->setMaximumHeight(280);
    ui->WRepositorStockModificar->setGeometry(0,0, 760, 280);
    ui->WRepositorStockEliminarProducto->setGeometry(270, 10, 481,261);
    ui->WRepositorStockModificar->show();
    ui->WRepositorStockEliminarProducto->show();
    ui->LblRepoEliminarCodigo2->setText(ui->TWRepoConsultarProductos->item(row, 0)->text());
    ui->LblRepoEliminarCantidad2->setText(ui->TWRepoConsultarProductos->item(row, 1)->text());
    ui->LblRepoEliminarMarca2->setText(ui->TWRepoConsultarProductos->item(row, 2)->text());
    ui->LblRepoEliminarProducto2->setText(ui->TWRepoConsultarProductos->item(row, 3)->text());
    ui->LblRepoEliminarDescripcion2->setText(ui->TWRepoConsultarProductos->item(row, 4)->text());
    ui->LblRepoEliminarPrecioCosto2->setText(ui->TWRepoConsultarProductos->item(row, 5)->text());
    ui->LblRepoEliminarImagen2->setText(ui->TWRepoConsultarProductos->item(row,6)->text());
    ui->LblRepoModificarMuestraImagen->setPixmap(QPixmap(ui->TWRepoConsultarProductos->item(row, 6)->text()));
    ui->LblRepoModificarMuestraImagen->setScaledContents(true);
}

void MainWindow::on_BtnRepoEliminarModificar_clicked()
{
    ui->WRepositorStockEliminarProducto->hide();
    ui->WRepositorStockModficarProducto->setGeometry(270,10,501, 261);
    ui->WRepositorStockModficarProducto->show();
    ui->LedRepoModificarCodigo_2->setText(ui->LblRepoEliminarCodigo2->text());
    //ui->LedRepoModificarCodigo_2->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(),this));
    ui->LedRepoModificarCantidad_2->setText(ui->LblRepoEliminarCantidad2->text());
    //ui->LedRepoModificarCantidad_2->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(),this));
    ui->LedRepoModificarMarca_2->setText(ui->LblRepoEliminarMarca2->text());
    ui->LedRepoModificarProducto_2->setText(ui->LblRepoEliminarProducto2->text());
    ui->LedRepoModificarDescripcion_2->setText(ui->LblRepoEliminarDescripcion2->text());
    ui->LedRepoModificarPrecioCosto_2->setText(ui->LblRepoEliminarPrecioCosto2->text());
    //ui->LedRepoModificarPrecioCosto_2->setValidator(new QIntValidator(0, std::numeric_limits<float>::max(),this));
    ui->LedRepoModificarImagen_2->setText(ui->LblRepoEliminarImagen2->text());
}

void MainWindow::on_BtnRepoEliminarEliminar_clicked()
{
    Productos prod;
    unsigned int prueba = ui->LblRepoEliminarCodigo2->text().toInt();
    QFile arch("arch/stock.txt");
    QFile arch1("arch/stock.tmp");
    if(arch.open(QIODevice::ReadOnly|QIODevice::Text)&& arch1.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QString renglon;
        unsigned int contador = 0;
        QTextStream out(&arch1);
        while(!arch.atEnd())
        {
            do
            {
            renglon = arch.readLine();
            renglon.chop(1);
            switch (contador) {
            case 0:
                prod.codigo = renglon.toInt();
                break;
            case 1:
                prod.cantidad = renglon.toInt();
                break;
            case 2:
                prod.marca = renglon;
                break;
            case 3:
                prod.producto = renglon;
                break;
            case 4:
                prod.descripcion = renglon;
                break;
            case 5:
                prod.costo = renglon.toFloat();
                break;
            case 6:
                prod.imagen = renglon;
                break;
            default:
                break;
            }
            contador++;
            }while(!renglon.isEmpty());
            if(prueba != prod.codigo && prod.codigo > 0)
            {
               out << "\n";
               out << prod.codigo << "\n";
               out << prod.cantidad << "\n";
               out << prod.marca << "\n";
               out << prod.producto << "\n";
               out << prod.descripcion << "\n";
               out << prod.costo << "\n";
               out << prod.imagen << "\n";
            }
            contador = 0;
        }
    }
    else
    {
        QMessageBox::information(NULL,"Error", "No se puede abrir el archivo.");
    }
    arch.close();
    arch1.close();

    QMessageBox quitMessageBox;
    quitMessageBox.setWindowTitle("Eliminar");
    //quitMessageBox.setWindowIcon(QIcon(":/new/img/Salir.gif"));
    quitMessageBox.setIcon(QMessageBox::Question);
    quitMessageBox.setText("Realmente desea Eliminar el producto de codigo "+ui->LblRepoEliminarCodigo2->text()+"?");
    quitMessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    quitMessageBox.setDefaultButton(QMessageBox::No);
    quitMessageBox.button(QMessageBox::Yes)->setText("Sí");
    quitMessageBox.button(QMessageBox::No)->setText("No");
    if(quitMessageBox.exec() == QMessageBox::Yes )
    {
        arch.remove();
        arch1.rename("arch/stock.txt");
        ui->TWRepoConsultarProductos->clearContents();
        ui->TWRepoConsultarProductos->setRowCount(0);
        QFile arch("arch/stock.txt");
        if(arch.open(QIODevice::ReadOnly|QIODevice::Text))
        {
        QString renglon;

        unsigned int datosStr = 0;
        unsigned int fila = 0;

        Productos prod;
        while(!arch.atEnd())
        {
            do{
                renglon = arch.readLine();
                renglon.chop(1);

                switch(datosStr)
                {
                case 0:
                    prod.codigo = renglon.toInt();
                    break;
                case 1:
                    prod.cantidad = renglon.toInt();
                    break;
                case 2:
                    prod.marca = renglon;
                    break;
                case 3:
                    prod.producto = renglon;
                    break;
                case 4:
                    prod.descripcion = renglon;
                    break;
                case 5:
                    prod.costo = renglon.toFloat();
                    break;
                case 6:
                    prod.imagen = renglon;
                    break;
                default: ;
                }
                datosStr++;

               }while(!renglon.isEmpty());
            if(prod.codigo > 0)
            {
                ui->TWRepoConsultarProductos->insertRow(fila);
                QTableWidgetItem *codigo = new QTableWidgetItem(tr("%1").arg(prod.codigo));
                codigo->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *cantidad = new QTableWidgetItem(tr("%1").arg(prod.cantidad));
                cantidad->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *marca = new QTableWidgetItem(prod.marca);
                marca->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *producto = new QTableWidgetItem(prod.producto);
                producto->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *descripcion = new QTableWidgetItem(prod.descripcion);
                descripcion->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *costo = new QTableWidgetItem(tr("%1").arg(prod.costo));
                costo->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *imagen = new QTableWidgetItem(prod.imagen);
                imagen->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                ui->TWRepoConsultarProductos->setItem(fila,0,codigo);
                ui->TWRepoConsultarProductos->setItem(fila,1,cantidad);
                ui->TWRepoConsultarProductos->setItem(fila,2,marca);
                ui->TWRepoConsultarProductos->setItem(fila,3,producto);
                ui->TWRepoConsultarProductos->setItem(fila,4,descripcion);
                ui->TWRepoConsultarProductos->setItem(fila,5,costo);
                ui->TWRepoConsultarProductos->setItem(fila,6,imagen);
                fila++;
            }

            datosStr = 0;
        }
        ui->WRepositor->show();
        ui->WRepoConsultarProd->show();
        this->setGeometry(200,100,800,600);
        this->setMinimumWidth(800);
        this->setMinimumHeight(600);
        this->setMaximumWidth(800);
        this->setMaximumHeight(600);
        }
        else
        {
            QMessageBox::information(NULL,"Error", "No se puede abrir el archivo.");
        }
        arch.close();
    }
    else
    {
    arch1.remove();
    }
    ui->WRepositorStockEliminarProducto->hide();
    ui->WRepositorStockModificar->hide();
    ui->WRepositor->show();
    this->setGeometry(200,100,800,600);
    this->setMinimumWidth(800);
    this->setMinimumHeight(600);
    this->setMaximumWidth(800);
    this->setMaximumHeight(600);
}

void MainWindow::on_BtnRepoModificarActualizar_2_clicked()
{
    Productos prod;
    unsigned int prueba = ui->LblRepoEliminarCodigo2->text().toInt();
    QFile arch("arch/stock.txt");
    QFile arch1("arch/stock.tmp");
    if(arch.open(QIODevice::ReadOnly|QIODevice::Text)&& arch1.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QString renglon;
        unsigned int contador = 0;
        QTextStream out(&arch1);
        while(!arch.atEnd())
        {
            do
            {
            renglon = arch.readLine();
            renglon.chop(1);
            switch (contador) {
            case 0:
                prod.codigo = renglon.toInt();
                break;
            case 1:
                prod.cantidad = renglon.toInt();
                break;
            case 2:
                prod.marca = renglon;
                break;
            case 3:
                prod.producto = renglon;
                break;
            case 4:
                prod.descripcion = renglon;
                break;
            case 5:
                prod.costo = renglon.toFloat();
                break;
            case 6:
                prod.imagen = renglon;
                break;
            default:
                break;
            }
            contador++;
            }while(!renglon.isEmpty());
            if(prueba == prod.codigo && prod.codigo != 0)
            {

               out << "\n";
               out << ui->LedRepoModificarCodigo_2->text().toInt() << "\n";
               out << ui->LedRepoModificarCantidad_2->text().toInt() << "\n";
               out << ui->LedRepoModificarMarca_2->text() << "\n";
               out << ui->LedRepoModificarProducto_2->text() << "\n";
               out << ui->LedRepoModificarDescripcion_2->text() << "\n";
               out << ui->LedRepoModificarPrecioCosto_2->text().toFloat() << "\n";
               out << ui->LedRepoModificarImagen_2->text() << "\n";
            }
            else if(prod.codigo != 0)
            {
                out << "\n";
                out << prod.codigo << "\n";
                out << prod.cantidad << "\n";
                out << prod.marca << "\n";
                out << prod.producto << "\n";
                out << prod.descripcion << "\n";
                out << prod.costo << "\n";
                out << prod.imagen << "\n";
            }
         contador = 0;
        }
    }
    else
    {
        QMessageBox::information(NULL,"Error", "No se puede abrir el archivo.");
    }
    arch.close();
    arch1.close();

    QMessageBox quitMessageBox;
    quitMessageBox.setWindowTitle("Eliminar");
    //quitMessageBox.setWindowIcon(QIcon(":/new/img/Salir.gif"));
    quitMessageBox.setIcon(QMessageBox::Question);
    quitMessageBox.setText("Realmente desea modificar el producto de codigo "+ui->LblRepoEliminarCodigo2->text()+"?");
    quitMessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    quitMessageBox.setDefaultButton(QMessageBox::No);
    quitMessageBox.button(QMessageBox::Yes)->setText("Sí");
    quitMessageBox.button(QMessageBox::No)->setText("No");
    if(quitMessageBox.exec() == QMessageBox::Yes )
    {
        arch.remove();
        arch1.rename("arch/stock.txt");
        ui->TWRepoConsultarProductos->clearContents();
        ui->TWRepoConsultarProductos->setRowCount(0);
        QFile arch("arch/stock.txt");
        if(arch.open(QIODevice::ReadOnly|QIODevice::Text))
        {
        QString renglon;

        unsigned int datosStr = 0;
        unsigned int fila = 0;

        Productos prod;
        while(!arch.atEnd())
        {
            do{
                renglon = arch.readLine();
                renglon.chop(1);

                switch(datosStr)
                {
                case 0:
                    prod.codigo = renglon.toInt();
                    break;
                case 1:
                    prod.cantidad = renglon.toInt();
                    break;
                case 2:
                    prod.marca = renglon;
                    break;
                case 3:
                    prod.producto = renglon;
                    break;
                case 4:
                    prod.descripcion = renglon;
                    break;
                case 5:
                    prod.costo = renglon.toFloat();
                    break;
                case 6:
                    prod.imagen = renglon;
                    break;
                default: ;
                }
                datosStr++;

               }while(!renglon.isEmpty());
            if(prod.codigo > 0)
            {
                ui->TWRepoConsultarProductos->insertRow(fila);
                QTableWidgetItem *codigo = new QTableWidgetItem(tr("%1").arg(prod.codigo));
                codigo->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *cantidad = new QTableWidgetItem(tr("%1").arg(prod.cantidad));
                cantidad->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *marca = new QTableWidgetItem(prod.marca);
                marca->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *producto = new QTableWidgetItem(prod.producto);
                producto->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *descripcion = new QTableWidgetItem(prod.descripcion);
                descripcion->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *costo = new QTableWidgetItem(tr("%1").arg(prod.costo));
                costo->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                QTableWidgetItem *imagen = new QTableWidgetItem(prod.imagen);
                imagen->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                ui->TWRepoConsultarProductos->setItem(fila,0,codigo);
                ui->TWRepoConsultarProductos->setItem(fila,1,cantidad);
                ui->TWRepoConsultarProductos->setItem(fila,2,marca);
                ui->TWRepoConsultarProductos->setItem(fila,3,producto);
                ui->TWRepoConsultarProductos->setItem(fila,4,descripcion);
                ui->TWRepoConsultarProductos->setItem(fila,5,costo);
                ui->TWRepoConsultarProductos->setItem(fila,6,imagen);
                fila++;
            }

            datosStr = 0;
        }
        ui->WRepositorStockModficarProducto->hide();
        ui->WRepositorStockModificar->hide();
        ui->WRepositor->show();
        ui->WRepoConsultarProd->show();
        this->setGeometry(200,100,800,600);
        this->setMinimumWidth(800);
        this->setMinimumHeight(600);
        this->setMaximumWidth(800);
        this->setMaximumHeight(600);
        }
        else
        {
            QMessageBox::information(NULL,"Error", "No se puede abrir el archivo.");
        }
        arch.close();
    }
    else
    {
    arch1.remove();
    }
}

void MainWindow::on_BtnRepoAgregarDirImagen_clicked()
{
    ui->LedRepoAgregarImagen->setText(QFileDialog::getOpenFileName(this, "Abrir archivo de imagen", QDir::currentPath(), "Fotos (*.png *.jpg *jpeg);;Gráficos (*.gif)"));
    ui->LblRepoAgregarMuestraImagen->setPixmap(QPixmap(ui->LedRepoAgregarImagen->text()));
    ui->LblRepoAgregarMuestraImagen->setScaledContents(true);

}

void MainWindow::on_BtnRepoModificarDirImagen_clicked()
{
    ui->LedRepoModificarImagen_2->setText(QFileDialog::getOpenFileName(this, "Abrir archivo de imagen", QDir::currentPath(), "Fotos (*.png *.jpg *jpeg);;Gráficos (*.gif)"));
    ui->LblRepoModificarMuestraImagen->setPixmap(QPixmap(ui->LedRepoModificarImagen_2->text()));
    ui->LblRepoModificarMuestraImagen->setScaledContents(true);
}

void MainWindow::on_BtnRepoEliminarVolver_clicked()
{
    ui->WRepositorStockModificar->hide();
    ui->WRepositor->show();
    ui->WRepoConsultarProd->show();
    this->setGeometry(200,100,800,600);
    this->setMinimumWidth(800);
    this->setMinimumHeight(600);
    this->setMaximumWidth(800);
    this->setMaximumHeight(600);
}

void MainWindow::on_BtnModificarSalir1_clicked()
{
    ui->WAdministrador->show();
    ui->WMuestraUsuariosAdmin->show();
    ui->WModificaUsuario->hide();
    ui->WModifica1Usuario->hide();
    this->setGeometry(200,100,800,600);
    this->setMinimumWidth(800);
    this->setMinimumHeight(600);
    this->setMaximumWidth(800);
    this->setMaximumHeight(600);
}

void MainWindow::on_BtnSalirRep_clicked()
{
    ui->WRepositor->hide();
    this->setGeometry(300,300,400,200);
    this->setMinimumWidth(400);
    this->setMinimumHeight(200);
    this->setMaximumWidth(400);
    this->setMaximumHeight(200);
    ui->WLoginInicio->show();
    ui->LedPass->clear();
    ui->LedUser->clear();
    ui->LedUser->setFocus();
}

void MainWindow::on_BtnSalirRep_2_clicked()
{
    ui->WVendedor->hide();
    this->setGeometry(300,300,400,200);
    this->setMinimumWidth(400);
    this->setMinimumHeight(200);
    this->setMaximumWidth(400);
    this->setMaximumHeight(200);
    ui->WLoginInicio->show();
    ui->LedPass->clear();
    ui->LedUser->clear();
    ui->LedUser->setFocus();
}

void MainWindow::on_BtnVendAdherirProd_clicked()
{
    unsigned int codigoVenta = 0, cantidadVenta = 0;
    codigoVenta = ui->LedVendCodigo->text().toInt();
    cantidadVenta = ui->LedVendCantidad->text().toInt();
    if(codigoVenta != 0)
    {
        if(cantidadVenta != 0)
        {
            if(ExisteCodigo(codigoVenta))
            {
                QFile arch("arch/stock.txt");
                QFile arch1("arch/stock.tmp");
                if(arch.open(QIODevice::ReadOnly|QIODevice::Text)&& arch1.open(QIODevice::WriteOnly|QIODevice::Text))
                {
                Productos prod;
                unsigned int contador = 0;
                unsigned int fila = ui->TWVendProductosVenta->rowCount();
                QTextStream out(&arch1);
                QString renglon;
                while(!arch.atEnd())
                {
                    do
                    {
                        renglon = arch.readLine();
                        renglon.chop(1);
                        switch (contador) {
                            case 0:
                            prod.codigo = renglon.toInt();
                            break;
                            case 1:
                            prod.cantidad = renglon.toInt();
                            break;
                            case 2:
                            prod.marca = renglon;
                            break;
                            case 3:
                            prod.producto = renglon;
                            break;
                            case 4:
                            prod.descripcion = renglon;
                            break;
                            case 5:
                            prod.costo = renglon.toFloat();
                            break;
                            case 6:
                            prod.imagen = renglon;
                            break;
                            default: ;
                            break;
                                }
                       contador++;
                    }while(!renglon.isEmpty());
                    contador = 0;
                if(codigoVenta == prod.codigo)
                {

                    ui->TWVendProductosVenta->insertRow(fila);
                    QTableWidgetItem *codigo = new QTableWidgetItem(tr("%1").arg(prod.codigo));
                    codigo->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *cantidad = new QTableWidgetItem(tr("%1").arg(cantidadVenta));
                    cantidad->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *marca = new QTableWidgetItem(prod.marca);
                    marca->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *producto = new QTableWidgetItem(prod.producto);
                    producto->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *descripcion = new QTableWidgetItem(prod.descripcion);
                    descripcion->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *costo = new QTableWidgetItem(tr("%1").arg("$"+QString::number(prod.costo, 'f', 2)));
                    costo->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *total = new QTableWidgetItem(tr("%1").arg("$"+QString::number(prod.costo*(float)cantidadVenta,'f', 2)));
                    total->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

                    ui->TWVendProductosVenta->setItem(fila,0,codigo);
                    ui->TWVendProductosVenta->setItem(fila,1,cantidad);
                    ui->TWVendProductosVenta->setItem(fila,2,marca);
                    ui->TWVendProductosVenta->setItem(fila,3,producto);
                    ui->TWVendProductosVenta->setItem(fila,4,descripcion);
                    ui->TWVendProductosVenta->setItem(fila,5,costo);
                    ui->lblVendImagenProd->setPixmap(QPixmap(prod.imagen));
                    ui->lblVendImagenProd->setScaledContents(true);
                    ui->TWVendProductosVenta->setItem(fila,6,total);

                    out << "\n";
                    out << prod.codigo << "\n";
                    out << prod.cantidad-cantidadVenta << "\n";
                    out << prod.marca << "\n";
                    out << prod.producto << "\n";
                    out << prod.descripcion << "\n";
                    out << prod.costo << "\n";
                    out << prod.imagen << "\n";
                    //QMessageBox::information(NULL,"Entro","codigo es igual a prod.codgo");
                    ui->LedVendMuestraProdPrecio->setText(prod.marca+" "+prod.producto+" $"+QString::number(prod.costo));
                    ui->lblVendTotal_2->setText(QString::number(ui->lblVendTotal_2->text().toFloat()+prod.costo*(float)cantidadVenta,'f', 2));
                    ui->LedVendPago->setText(QString::number(0.0));
                    }
                    else if (prod.codigo > 0)
                    {
                    out << "\n";
                    out << prod.codigo << "\n";
                    out << prod.cantidad << "\n";
                    out << prod.marca << "\n";
                    out << prod.producto << "\n";
                    out << prod.descripcion << "\n";
                    out << prod.costo << "\n";
                    out << prod.imagen << "\n";
                    }
                }
                }
                else
                {
                    QMessageBox::information(NULL, "Error", "No se ha podido abrir el archvo.");
                }
                arch.close();
                arch1.close();
        }
        else
        {
            QMessageBox::information(NULL,"Error","No existe el codigo.");
            ui->LedVendCantidad->clear();
            ui->LedVendCodigo->clear();
            ui->LedVendCodigo->setFocus();
        }
        }
        else
        {
            QMessageBox::information(NULL,"Error", "No se coloco la cantidad.");
            ui->LedVendCantidad->setFocus();
        }
    }
    else
    {
        QMessageBox::information(NULL, "Error", "Falta el codigo de producto.");
        ui->LedVendCodigo->setFocus();
    }
    ui->LedVendCantidad->clear();
    ui->LedVendCodigo->clear();
    ui->LedVendCodigo->setFocus();
}

void MainWindow::on_LedVendPago_returnPressed()
{
    float vuelto = 0;
    vuelto = ui->LedVendPago->text().toFloat()-ui->lblVendTotal_2->text().toFloat();
    QString vuel = QString::number(vuelto, 'f', 2);
    ui->lblVendVuelto_2->setText(vuel);
}

void MainWindow::on_BtnVendFinalizarCompra_clicked()
{
    QFile arch2("arch/ventas.txt");
    QFile arch1("arch/stock.tmp");
    QFile arch("arch/stock.txt");
    QTextStream out(&arch2);
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer,NULL);

    if(arch2.open(QIODevice::Append|QIODevice::Text))
    {
        if(arch2.atEnd())
        {
        out << "\n";
        out << ui->lblVendFecha->text() << "\n";
        out << ui->lblVendHora->text() << "\n";
        out << "$"+ui->lblVendTotal_2->text() << "\n";
        }
    }
    else
    {
        QMessageBox::information(NULL,"Error","El archivo Ventas.txt no pudo ser abierto.");
    }
    arch2.close();
    arch.remove();
    arch1.rename("arch/stock.txt");

    ui->TxtEVendFacturaCreada->append("   <<<     Quick E Mark    >>>    \n\n\n"
                                      "APU NAJASAPEMAMETILAN       \n\n"
                                      "CUIT Nro.: 22-22222222-2    \n\n"
                                      "ING.BRUTOS: 22-22222222-2   \n\n"
                                      "Calle Falsa 123 - (2222) SPRINGFIELD\n\n"
                                      "INICIO DE ACTIVIDAD: 03/2015\n\n"
                                      "IVA RESPONSABLE INSCRIPTO   \n\n"
                                      "-------------------------------------------\n"
                                      "TIQUE FACTURA B          Nro. 0002-22222222\n"
                                      "                        "+ui->lblVendFecha->text()+"\n"
                                      "                        "+ui->lblVendHora->text()+"\n"
                                      "-------------------------------------------\n"
                                      "CONSUMIDOR FINAL            \n"
                                      "VENDEDOR: "+ui->LblVendedor->text()+"    \n"
                                      ""+ui->lblfechaInVend->text()+"              \n"
                                      "---------------------------------------------\n"
                                      "Cant./Precio Unit.                           \n"
                                      "Descripcion (%IVA)[%BI]               IMPORTE\n"
                                      "---------------------------------------------\n");
    int contador = 0;
    int articulos = ui->TWVendProductosVenta->rowCount();
    //QMessageBox::information(NULL,"Error", QString::number(articulos));
    do{
        ui->TxtEVendFacturaCreada->append(""+ui->TWVendProductosVenta->item(contador,1)->text()+" x "+ui->TWVendProductosVenta->item(contador,5)->text()+"      \n"
                                          ""+ui->TWVendProductosVenta->item(contador,0)->text()+" "+ui->TWVendProductosVenta->item(contador,2)->text()+" "+ui->TWVendProductosVenta->item(contador,3)->text()+" "+ui->TWVendProductosVenta->item(contador,6)->text()+"\n");
        contador++;
    }while(contador!=ui->TWVendProductosVenta->rowCount());
    ui->TxtEVendFacturaCreada->append("\n"
                                      "----------------------------------------------\n"
                                      "TOTAL                   $"+ui->lblVendTotal_2->text()+"\n"
                                      "\n"
                                      "MONTO DE PAGO           $"+ui->LedVendPago->text()+"\n"
                                      "SUMA DE SUS PAGOS       $"+ui->LedVendPago->text()+"\n"
                                      "SU VUELTO               $"+ui->lblVendVuelto_2->text()+"\n"
                                      "\n"
                                      "CANTIDAD DE ARTICULOS "+QString::number(articulos)+"\n"
                                      "\n"
                                      "\n"
                                      "<<<<<<<<<<<<GRACIAS>>>>>>>>>>>>\n"
                                      "<<<<<<<<<POR SU VISITA>>>>>>>>\n");

    if(printDialog.exec() == QDialog::Accepted)
    {
        ui->TxtEVendFacturaCreada->print(&printer);
    }
    ui->lblVendVuelto_2->clear();
    ui->LedVendPago->clear();
    ui->lblVendTotal_2->clear();
    ui->TWVendProductosVenta->clearContents();
    ui->TWVendProductosVenta->setRowCount(0);
    ui->LedVendMuestraProdPrecio->clear();
    ui->LedVendCantidad->clear();
    ui->LedVendCodigo->clear();
    ui->LedVendCodigo->setFocus();
    ui->lblVendImagenProd->clear();
    ui->TxtEVendFacturaCreada->clear();
}

void MainWindow::on_BtnVendPagar_clicked()
{
    ui->LedVendPago->setDisabled(false);
}

void MainWindow::on_TBtnAltaFecha_3_clicked()
{
    ui->CalModifica->show();
}

void MainWindow::on_CalModifica_clicked(const QDate &date)
{
    ui->LedModificaFechIngreso->setText(date.toString("dd-MM-yyyy"));
    ui->CalModifica->hide();
}

void MainWindow::on_BtnInforme_clicked()
{
    ui->TWAdminMuestraUsuarios->hide();
    ui->BtnAlta->hide();
    ui->LblAdminDesde->show();
    ui->LedAdminFechaDesde->show();
    ui->BtnAdminBuscarInforme->show();
    ui->toolButton->show();
    ui->LblAdminHasta->show();
    ui->LedAdminFechaHasta->show();
    ui->toolButton_2->show();
    ui->BtnInforme->setDisabled(true);
}

void MainWindow::on_toolButton_clicked()
{
    ui->CalAdminInformeDesde->show();
}

void MainWindow::on_toolButton_2_clicked()
{
    ui->CalAdminInformHasta->show();
}

void MainWindow::on_CalAdminInformeDesde_clicked(const QDate &date)
{
    ui->LedAdminFechaDesde->setText(date.toString("dd-MM-yyyy"));
    ui->CalAdminInformeDesde->hide();
}

void MainWindow::on_CalAdminInformHasta_clicked(const QDate &date)
{
    ui->LedAdminFechaHasta->setText(date.toString("dd-MM-yyyy"));
            ui->CalAdminInformHasta->hide();
}
