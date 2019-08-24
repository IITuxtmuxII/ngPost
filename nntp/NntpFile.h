#ifndef NntpFile_H
//========================================================================
//
// Copyright (C) 2019 Matthieu Bruel <Matthieu.Bruel@gmail.com>
//
// This file is a part of ngPost : https://github.com/mbruel/ngPost
//
// ngPost is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 3.0 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// You should have received a copy of the GNU Lesser General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301,
// USA.
//
//========================================================================

#define NntpFile_H
#include <QObject>
#include <QVector>
#include <QFileInfo>
class NntpArticle;
class QTextStream;

/*!
 * \brief The NntpFile class represent a File uploaded on Usenet thus it will holds all its Articles
 * All NntpFiles will live (active objects) in the Main Thread so they can write into the nzb without concurrency
 * when all its articles are posted (slot onArticlePosted) it emits allArticlesArePosted
 */
class NntpFile : public QObject
{
    Q_OBJECT

public:
    NntpFile(const QFileInfo &file, int num, int nbFiles, const QVector<QString> &grpList);
    ~NntpFile();

    inline void addArticle(NntpArticle *article);

    void writeToNZB(QTextStream &stream);

    inline QString path() const;
    inline QString name() const;
    inline std::string fileName() const;
    inline qint64 fileSize() const;
    inline int nbArticles() const;

signals:
    void allArticlesArePosted(NntpFile *NntpFile);

public slots:
    void onArticlePosted(NntpArticle *article);

private:
    const QFileInfo         _file;      //!< original file
    const int               _num;       //!< file number
    const int               _nbFiles;   //!< total number of file
    const QVector<QString> &_grpList;   //!< groups where the file is posted
    const int               _nbAticles; //!< total number of articles
    QVector<NntpArticle*>   _articles;  //!< all articles (that are yEnc encoded)

    int _nbPosted; //!< number of Articles that have been posted (uploaded on the socket)
};

void NntpFile::addArticle(NntpArticle *article) { _articles.push_back(article); }
QString NntpFile::path() const { return _file.absoluteFilePath(); }
QString NntpFile::name() const { return QString("[%1/%2] %3").arg(_num).arg(_nbFiles).arg(_file.fileName()); }
std::string NntpFile::fileName() const { return _file.fileName().toStdString(); }
qint64 NntpFile::fileSize() const { return _file.size(); }
int NntpFile::nbArticles() const { return _nbAticles; }

#endif // NntpFile_H
