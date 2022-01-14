From de284dbd524aab88d71b9ec9798a9cc7de010283 Mon Sep 17 00:00:00 2001
From: ImSpiDy <spidy2713@gmail.com>
Date: Wed, 19 Jan 2022 15:33:53 +0000
Subject: [PATCH] Revert "lavender: defconfig: Disable xt_qtaguid"

This reverts commit cd7cdf9b7fa439d13374b4ff27d75e752f60f83f.
---
 arch/arm64/configs/lavender-perf_defconfig | 4 ++--
 1 file changed, 2 insertions(+), 2 deletions(-)

diff --git a/arch/arm64/configs/lavender-perf_defconfig b/arch/arm64/configs/lavender-perf_defconfig
index 81799ca239e2..da0d40df24e5 100644
--- a/arch/arm64/configs/lavender-perf_defconfig
+++ b/arch/arm64/configs/lavender-perf_defconfig
@@ -908,10 +908,10 @@ CONFIG_NETFILTER_XT_MATCH_MARK=y
 CONFIG_NETFILTER_XT_MATCH_MULTIPORT=y
 # CONFIG_NETFILTER_XT_MATCH_NFACCT is not set
 # CONFIG_NETFILTER_XT_MATCH_OSF is not set
-CONFIG_NETFILTER_XT_MATCH_OWNER=y
+# CONFIG_NETFILTER_XT_MATCH_OWNER is not set
 CONFIG_NETFILTER_XT_MATCH_POLICY=y
 CONFIG_NETFILTER_XT_MATCH_PKTTYPE=y
-# CONFIG_NETFILTER_XT_MATCH_QTAGUID is not set
+CONFIG_NETFILTER_XT_MATCH_QTAGUID=y
 CONFIG_NETFILTER_XT_MATCH_QUOTA=y
 CONFIG_NETFILTER_XT_MATCH_QUOTA2=y
 # CONFIG_NETFILTER_XT_MATCH_QUOTA2_LOG is not set
-- 
2.20.1

