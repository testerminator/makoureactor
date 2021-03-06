#ifndef SEARCHALL_H
#define SEARCHALL_H

#include <QtWidgets>

class Window;
class FieldArchive;

class SearchAll : public QDialog
{
	Q_OBJECT
public:
	explicit SearchAll(Window *parent);
	void setScriptSearch();
	void setTextSearch();
	void setFieldArchive(FieldArchive *fieldArchive);
	void clear();
public slots:
	void addResultOpcode(int fieldID, int grpScriptID, int scriptID, int opcodeID);
	void addResultText(int fieldID, int textID, int index, int size);
private slots:
	void gotoResult(QTreeWidgetItem *item);
	void copySelected() const;
private:
	enum Mode {
		ScriptSearch, TextSearch
	};

	inline Window *mainWindow() const {
		return reinterpret_cast<Window *>(parentWidget());
	}
	void addResult(int fieldID, QTreeWidgetItem *item);
	QTreeWidgetItem *createItemField(int fieldID) const;
	QTreeWidgetItem *createItemOpcode(int fieldID, int grpScriptID, int scriptID, int opcodeID) const;
	QTreeWidgetItem *createItemText(int fieldID, int textID, int index, int size) const;
	QTreeWidget *_resultList;
	Mode _searchMode;
	FieldArchive *_fieldArchive;
	QMap<int, QTreeWidgetItem *> itemByFieldID;
};

#endif // SEARCHALL_H
