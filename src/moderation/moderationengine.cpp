#include "moderationengine.h"
#include "linkvalidator.h"
#include <QDebug>
#include <QFile>
#include <QRegularExpression>

ModerationEngine::ModerationEngine()
    : m_linkValidator(std::make_unique<LinkValidator>())
{
    initializeBlacklist();
}

ModerationEngine::~ModerationEngine() = default;

bool ModerationEngine::validateLink(const QString &url)
{
    return !isMaliciousLink(url);
}

bool ModerationEngine::validateImage(const QByteArray &imageData)
{
    // TODO: Implement image content validation
    // - Check file type and magic bytes
    // - Scan for inappropriate content
    // - Verify image dimensions
    
    if (imageData.isEmpty()) {
        return false;
    }
    
    // Basic validation: check for common image magic bytes
    if (imageData.startsWith("\xFF\xD8\xFF")) {  // JPEG
        return true;
    }
    if (imageData.startsWith("\x89PNG")) {  // PNG
        return true;
    }
    if (imageData.startsWith("GIF8")) {  // GIF
        return true;
    }
    if (imageData.startsWith("\x00\x00\x01\x00")) {  // ICO
        return true;
    }
    
    return false;
}

QString ModerationEngine::filterContent(const QString &content)
{
    QString filtered = content;
    
    // Extract and validate links
    QStringList links = extractLinks(content);
    for (const QString &link : links) {
        if (isMaliciousLink(link)) {
            filtered.replace(link, "[REMOVED - MALICIOUS LINK]");
        }
    }
    
    return filtered;
}

QStringList ModerationEngine::extractLinks(const QString &text)
{
    QStringList links;
    
    QRegularExpression urlRegex("https?://[^\\s]+");
    QRegularExpressionMatchIterator it = urlRegex.globalMatch(text);
    
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        links.append(match.captured(0));
    }
    
    return links;
}

void ModerationEngine::loadBlacklist(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open blacklist file:" << filePath;
        return;
    }
    
    m_blacklist.clear();
    while (!file.atEnd()) {
        QString line = QString::fromUtf8(file.readLine()).trimmed();
        if (!line.isEmpty() && !line.startsWith("#")) {
            m_blacklist.append(line);
        }
    }
    
    file.close();
    qDebug() << "Blacklist loaded:" << m_blacklist.size() << "entries";
}

void ModerationEngine::updateBlacklist()
{
    // TODO: Implement periodic blacklist updates from remote source
    qDebug() << "Updating blacklist...";
}

bool ModerationEngine::isMaliciousLink(const QString &url) const
{
    // Check against local blacklist
    for (const QString &pattern : m_blacklist) {
        if (url.contains(pattern, Qt::CaseInsensitive)) {
            qWarning() << "Malicious link detected:" << url;
            return true;
        }
    }
    
    // Check trust score
    float trustScore = getLinkTrustScore(url);
    if (trustScore < 0.8f) {
        return true;
    }
    
    return false;
}

float ModerationEngine::getLinkTrustScore(const QString &url) const
{
    // TODO: Implement trust score calculation based on:
    // - Domain reputation
    // - URL length
    // - Redirect chains
    // - SSL certificate validity
    // - Domain age
    
    // Default to moderate trust for now
    return 0.9f;
}

void ModerationEngine::initializeBlacklist()
{
    // Initialize with some common malicious domains
    m_blacklist << "malicious.com"
                << "phishing.net"
                << "scam.org";
    
    // Whitelist major domains
    m_whitelistDomains << "roblox.com"
                       << "discord.com"
                       << "twitch.tv"
                       << "youtube.com"
                       << "github.com";
    
    qDebug() << "ModerationEngine initialized";
}
