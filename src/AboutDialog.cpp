#include "AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent)
: QDialog(parent, Qt::MSWindowsFixedSizeDialogHint|Qt::WindowTitleHint|Qt::WindowSystemMenuHint)
{
	setupUi(this);
}

AboutDialog::~AboutDialog()
{
}
