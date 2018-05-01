#include "scribusAPIDocumentItem.h"

#include <QDebug>

#include <QString>
#include <QSize>

#include "util_formats.h" // for checking file extension

#include "scribusdoc.h"
#include "marks.h" // for footnotes
#include "util_text.h" // for footnotes and desaxeString

#include "scribusAPIDocumentItemText.h"

void ScribusAPIDocumentItem::render()
{
    scribusItem->update();
}

/**
 * TODO:
 * - as soon as other write directions are to be considered the order has to be made more flexible
 * - we could have fancier rules for the position
 */
bool ScribusAPIDocumentItem::isBefore(ScribusAPIDocumentItem* const item) const
{
    return (getY() < item->getY()) ||
           ((getY() == item->getY()) && (getX() <= item->getX()));
}

ScribusAPIDocumentItemText* ScribusAPIDocumentItem::getText()
{
    return new ScribusAPIDocumentItemText(this);
}

/**
 * @return the visible part of the image in PNG or JPG format
 */
ScribusAPIDocumentItemImageWeb ScribusAPIDocumentItem::getImageWeb()
{
    ScribusAPIDocumentItemImageWeb result;

    QString filename(scribusItem->Pfile);
    // qDebug() << "filename" << filename;
    if (filename == "")
        return result;
    result.path = filename;

    QFileInfo fileinfo = QFileInfo(filename);
    QString ext = fileinfo.suffix().toLower();

    // TODO: sanitize the filename
    // zippedFilename.remove(QRegExp("[^a-zA-Z\\d\\s_.-]"));
    result.filename = fileinfo.fileName();

    int mediaType = 0;
    if (ext == "png")
        mediaType = FormatsManager::PNG;
    else if (extensionIndicatesJPEG(ext))
        mediaType = FormatsManager::JPEG;
    // qDebug() << "mediaType" << mediaType;

    if (mediaType == 0)
    {
        // TODO: convert the other "acceptable" image types to png or jpeg (how to choose?)
        qDebug() << "image format not yet supported: " << filename;
        return result;
    }

    result.mediatype = FormatsManager::instance()->mimetypeOfFormat(mediaType).first();

    QPixmap image; // empty if the image has not been cropped nor scaled
    bool usingLoadedImage = false;

    // qDebug() << "image file" << filename;
    // qDebug() << "imageXScale" << scribusItem->imageXScale();
    // qDebug() << "imageYScale" << scribusItem->imageYScale();
    // qDebug() << "imageXOffset" << scribusItem->imageXOffset();
    // qDebug() << "imageYOffset" << scribusItem->imageYOffset();
    // qDebug() << "item width" << scribusItem->width();
    // qDebug() << "item height" << scribusItem->height();
    // qDebug() << "image BBoxX" << scribusItem->pixm.imgInfo.BBoxX;
    // qDebug() << "image BBoxH" << scribusItem->pixm.imgInfo.BBoxH;
    // qDebug() << "image xres" << scribusItem->pixm.imgInfo.xres;
    // qDebug() << "image yres" << scribusItem->pixm.imgInfo.yres;
    // qDebug() << "image width" << scribusItem->pixm.width();
    // qDebug() << "image height" << scribusItem->pixm.height();

    double cropX = scribusItem->imageXOffset();
    double cropY =  scribusItem->imageYOffset();

    // calculate the frame's width and height in "image pixels"
    double frameWidth = scribusItem->width() / scribusItem->imageXScale();
    double frameHeight = scribusItem->height() / scribusItem->imageYScale();

    result.imageSize = QSize(static_cast<int>(frameWidth), static_cast<int>(frameHeight));

     // TODO: if the image's width and height are already stored, only load the image when it has to be cropped or scaled
    if (!image.load(filename))
        return result;

    QRect frameRect = QRect(- cropX, -cropY, frameWidth, frameHeight);
    QRect imageRect = QRect(0, 0, image.width(), image.height());
    QRect cropRect;
    // qDebug() << "frameRect" << frameRect;
    // qDebug() << "imageRect" << imageRect;

    if (frameRect != imageRect)
    {
        usingLoadedImage = true;
        cropRect = frameRect.intersected(imageRect);
        // qDebug() << "cropRect" << cropRect;
        if (!cropRect.isEmpty())
        {
            QPixmap imageTmp = image.copy(cropRect);
            image = imageTmp;
        }
    }
    /*

    qDebug() << "frameWidth" << frameWidth;
    qDebug() << "imageMaxWidth" << imageMaxWidth;
    int scaling = 100;

    ScPage* page = doc->DocPages.at(this->pageNumber);
    qDebug() << "item width" << scribusItem->width();
    double proportion = scribusItem->width() / (page->width() - page->rightMargin() - page->leftMargin());
    qDebug() << "proportion" << proportion;
    qDebug() << "imageMaxWidthThreshold" << imageMaxWidthThreshold;

    int scaledWidth;
    if (proportion > static_cast<double>(imageMaxWidthThreshold) / 100)
        scaledWidth = imageMaxWidth;
    else
        scaledWidth = static_cast<int>(proportion * imageMaxWidth);
    qDebug() << "scaledWidth" << scaledWidth;

    qDebug() << "image width" << image.width();
    if (scaledWidth < frameWidth)
        scaling = static_cast<int>(static_cast<double>(scaledWidth) / image.width() * 100);
    qDebug() << "scaling" << scaling;

    QString zippedFilename = fileinfo.fileName();
    if (usingLoadedImage)
    {
        zippedFilename = fileinfo.completeBaseName()+"_c-%1-%2-%3-%4-s-%5."+fileinfo.suffix();
        zippedFilename = zippedFilename.arg(cropRect.x()).arg(cropRect.y()).arg(cropRect.width()).arg(cropRect.height()).arg(scaling);
    }
    zippedFilename.remove(QRegExp("[^a-zA-Z\\d\\s_.-]"));
    qDebug() << "zippedFilename" << zippedFilename;

    QString filepath = "Images/" + zippedFilename;

    // add the image to the dom
    QDomElement div = xhtmlDocument.createElement("div");
    // TODO: only set class picture if the images is maximized (or use different names: the goal is not to only set the page break before, if the picture is maximized)
    div.setAttribute("class", "picture");
    QDomElement element = xhtmlDocument.createElement("img");
    // <image height="800" width="600" xlink:href="../Images/cover.jpeg"></image>
    element.setAttribute("height", (int) scribusItem->height()); // TODO: use the real width of the visible part of the image (as a rectangle)
    element.setAttribute("width", (int) scribusItem->width());
    element.setAttribute("alt", ""); // TODO do we have a way to define the metadata? eventually from the exif? epubcheck says it's mandatory... and it's not nice to leave it empty...
    element.setAttribute("src", "../"+filepath); // TODO: make sure that the name is unique in the target! (if it already exists prefix the frame name?)
    // TODO: set the width and height? from the item?
    div.appendChild(element);

    result.append(div);
    */


    /*
    void EpubExport::addImage(PageItem* docItem)
    {


        // TODO: add also the path to the original picture before the zippedFilename in order to catch files with
        // the same name but from different paths... and then? how to set the name?
        if (!imageFileNames.contains(zippedFilename))
        {
            imageFileNames << zippedFilename;
            qDebug() << "zippedFilename" << zippedFilename;
            qDebug() << "imageFileNames" << imageFileNames;
            if (scaling > 0 && scaling < 100)
            {
                usingLoadedImage = true;
                QPixmap imageTmp = image.scaledToWidth(scaledWidth, Qt::SmoothTransformation);
                image = imageTmp;
            }

            // copy the image into the zip
            if (!usingLoadedImage)
            {
                qDebug() << "standard file add";
                QFile file(fileinfo.filePath()); // TODO: if we already have a scimage we may have to change this
                epubFile->add("OEBPS/"+filepath, &file, true);
            }
            else
            {
                QByteArray imageBytes;
                QBuffer buffer(&imageBytes);
                buffer.open(QIODevice::WriteOnly);
                image.save(&buffer, mediaType == FormatsManager::JPEG ? "jpg" : "png");
                epubFile->add("OEBPS/"+filepath, imageBytes, false);


            }

            struct EPUBExportContentItem contentItem;
            contentItem.id = zippedFilename;
            contentItem.href = filepath;
            contentItem.mediaType = FormatsManager::instance()->mimetypeOfFormat(mediaType).first();
            contentItems.append(contentItem);
        } // if (imageFileNames.contains(zippedFilename))
    }
*/
        /*
        // TODO: some leftovers if we want ever do a color managed conversion of the pictures
        ScImage img;
        ScImage scribusItem->pixm;
        ImageInfoRecord imgInfo;
            ImageTypeEnum type -> 0 = jpg, 1 = tiff, 2 = psd, 3 = eps/ps, 4 = pdf, 5 = jpg2000, 6 = other
            ColorSpaceEnum colorspace -> 0 = RGB  1 = CMYK  2 = Grayscale 3 = Duotone
        CMSettings cms(c->doc(), Profil, Intent);
        cms.setUseEmbeddedProfile(Embedded);
        usingLoadedImage = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, 72, &realCMYK);
        bool loadPicture(const QString & fn, int page, const CMSettings& cmSettings, RequestType requestType, int gsRes, bool *realCMYK = 0, bool showMsg = false);
         */

    return result;
}

QList<ScribusAPIDocumentItemResourceFile> ScribusAPIDocumentItem::getResourceFiles()
{
    QList<ScribusAPIDocumentItemResourceFile> result;
    return result;
}

QDebug operator<<(QDebug dbg, ScribusAPIDocumentItem &item)
{
    dbg.nospace() << "(name:" << item.getName() << ")";
    return dbg.space();
}

/**
 * @todo: do not print the full content!
 */
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemResourceFile file)
{
    dbg.nospace() << "(filename:" << file.filename << ", filePath:" << file.filePath  << ", fileContent:" << file.fileContent << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemImageWeb image)
{
    dbg.nospace() << "(\npath:" << image.path << ",\nfilename:" << image.filename << ",\ncontent:" << image.content  << ",\ndescription:" << image.description << ",\nimageSize:" << image.imageSize << ",\nmediatype:" << image.mediatype << "\n)";
    return dbg.space();
}
