<?xml version="1.0" encoding="iso-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- Heldenbrief V1.01 GMy -->
<xsl:template match="/XDIML/Inhalt/Personen">
  <html>
  <head>
  </head>
  <body bgcolor="#f0f0e0" text="#000000" topmargin="0" leftmargin="0" marginwidth="0" marginheight="0">
  <font face="Arial">
  <xsl:apply-templates select="Person" />
  </font>
  </body>
  </html>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                        Globale Variablen                                  -->
<!-- *************************************************************************** -->

<xsl:variable name="AttBasis" select="/XDIML/Inhalt/Personen/Person/AbgeleiteteWerte/AbgeleiteterWert[@ID='AttackeBasis']/Wert"></xsl:variable>
<xsl:variable name="PaBasis"  select="/XDIML/Inhalt/Personen/Person/AbgeleiteteWerte/AbgeleiteterWert[@ID='ParadeBasis']/Wert"></xsl:variable>
<xsl:variable name="FkBasis"  select="/XDIML/Inhalt/Personen/Person/AbgeleiteteWerte/AbgeleiteterWert[@ID='FernkampfBasis']/Wert"></xsl:variable>
<xsl:variable name="IniBasis" select="/XDIML/Inhalt/Personen/Person/AbgeleiteteWerte/AbgeleiteterWert[@ID='InitiativeBasis']/Wert"></xsl:variable>
<xsl:variable name="AE" select="/XDIML/Inhalt/Personen/Person/AbgeleiteteWerte/AbgeleiteterWert[@ID='Astralenergie']/Wert"></xsl:variable>

<xsl:template match="Person">

<!-- *************************************************************************** -->
<!-- *                          Personalien                                      -->
<!-- *************************************************************************** -->

<table cols="2" cellpadding="3" cellspacing="0" height="140" width="640">
    <tr valign="bottom" align="left">
       <td width="70%">Name: <xsl:apply-templates select="Name" /></td>
       <td width="30%">Geschlecht: <xsl:value-of select="Geschlecht" /></td>
    </tr>
    <tr valign="bottom" align="left">
       <td width="70%">Rasse: <xsl:value-of select="Rasse/Bezeichnung" /></td>
       <td width="30%">Alter: <xsl:value-of select="Alter" /></td>
    </tr>
    <tr valign="bottom" align="left">
       <td width="70%">Kultur: <xsl:value-of select="Kultur/Bezeichnung" /></td>
       <td width="30%">Größe: <xsl:value-of select="Groesse" /></td>
    </tr>
    <tr valign="bottom" align="left">
       <td width="70%">Profession: <xsl:value-of select="Profession/Bezeichnung" /></td>
       <td width="30%">Gewicht: <xsl:value-of select="Gewicht" /></td>
    </tr>
    <tr valign="bottom" align="left">
       <td width="70%">Aussehen: Hauptfarbe: <xsl:value-of select="Hauptfarbe" />,
                                  Augenfarbe: <xsl:value-of select="Augenfarbe" /></td>
       <td width="30%">Vermögen: </td>
    </tr>
</table>
<br />

<!-- *************************************************************************** -->
<!-- Überschrift für Eigenschaften und Kampftechniken                            -->
<!-- *************************************************************************** -->

<table cols="2" cellpadding="2" cellspacing="0" border="0" width="640">
<tr>
  <td width="50%"><b>Eigenschaften</b></td>
  <td width=",*"><b>Kampftechniken</b></td>
</tr>
</table>

<!-- *************************************************************************** -->
<!-- Tabelle für Eigenschaften und Kampftechniken                                -->
<!-- *************************************************************************** -->

<table cols="3" cellpadding="0" cellspacing="0" border="0" width="640">
  <tr>
      <td valign="top" width="160">
           <xsl:apply-templates select="Eigenschaften" />
      </td>  
      <td valign="top" width="160">
           <xsl:apply-templates select="AbgeleiteteWerte" />
      </td>    
      <td valign="top" width="320">
           <xsl:apply-templates select="Kampftechniken" />
      </td>  
  </tr>  
</table>

<!-- *************************************************************************** -->
<!-- *                   Charakteristika und Sonderfertigkeiten                  -->
<!-- *************************************************************************** -->

<br />
<table cols="2" cellpadding="2" cellspacing="0" border="0" width="640">
  <tr>
    <td width="50%"><b>Vorteile / Nachteile</b></td>
    <td width=",*"><b>Sonderfertigkeiten</b></td>
  </tr>
</table>

<table cols="2" cellpadding="0" cellspacing="0" border="0" width="640">
  <tr>
    <td valign="top" width="50%">
      <xsl:apply-templates select="Charakteristika" />
    </td>
    <td valign="top" width=",*">
      <xsl:apply-templates select="Sonderfertigkeiten[@ID='tatsächliche']" />
    </td>
  </tr>  
</table>   

<!-- *************************************************************************** -->
<!-- *                                 Waffen                                    -->
<!-- *************************************************************************** -->

<br />
<b>Nahkampfwaffen</b>
<table cols="10" cellpadding="2" cellspacing="0" border="1" width="640">
  <tr STYLE="font-size:11pt">
    <th width="33%">Waffe</th>
    <th width="10%">Typ/eBE</th>
    <th width="5%">DK </th>
    <th width="7%">TP</th>
    <th width="7%">TP/KK</th>
    <th width="5%">INI</th>
    <th width="7%">WM</th>
    <th width="6%">AT</th>
    <th width="6%">PA</th>
    <th width="8%">TPeff</th>
    <th width="6%">BF</th>
  </tr>
  <xsl:apply-templates select="Waffen/Waffengruppe[@ID='Nahkampfwaffen']" />
</table>
<br />

<b>Fernkampfwaffen</b>
<table cols="10" cellpadding="2" cellspacing="0" border="1" width="640">
  <tr STYLE="font-size:11pt">
    <th width="40%">Waffe</th>
    <th width="10%">Typ/eBE</th>
    <th width="7%">TP</th>
    <th width="10%">Entfernungen</th>
    <th width="10%">TP/Entfernung</th>
    <th width="6%">FK</th>
    <th width="40%">Geschosse</th>
  </tr>
  <xsl:apply-templates select="Waffen/Waffengruppe[@ID='Fernkampfwaffen']" />
</table>

<!-- *************************************************************************** -->
<!-- *                      Rüstungen und Schilde                                -->
<!-- *************************************************************************** -->

<br />
<table cols="2" width="640" cellpadding="0" cellspacing="0" border="0">
  <tr >
    <td valign="bottom" width="50%"><b>Rüstungen</b></td>
    <td width=",*"><b>Schilde</b></td>
  </tr>  
  <tr valign="top" align="left">
     <td>
       <table cellpadding="2" cellspacing="0" border="1" width="320">
         <tr STYLE="font-size:11pt">
           <td width="58%">Rüstungsstück</td>
           <td width="20%">RS</td>
           <td width="22%">BE(-INI)</td>
         </tr>
         <xsl:apply-templates select="Ruestungen" />
       </table>
     </td>
     <td>
       <table cellpadding="2" cellspacing="0" border="1" width="320">
         <tr STYLE="font-size:11pt">
           <td width="70%">Schild</td>
           <td width="15%">INI</td>
           <td width="15%">WM</td>
         </tr>
         <xsl:apply-templates select="Schilde" />
       </table>
     </td>
   </tr>
</table>
<br />

<!-- *************************************************************************** -->
<!-- *      Tabelle: Lebensenergie. Ausdauer, Initiative, AE und Wunden          -->
<!-- *************************************************************************** -->

<b>Variable Spielwerte</b>
<table cols="15" cellpadding="1" cellspacing="0" border="1" width="640">
  <tr STYLE="font-size:10pt">
     <td width="10%">Lebensenergie</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>     
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
  </tr>   
  <tr STYLE="font-size:10pt">
     <td width="10%">Ausdauer</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>     
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
  </tr>   
  <tr STYLE="font-size:10pt">
     <td width="10%">Initiative</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>     
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
  </tr>   
  <tr STYLE="font-size:10pt">
     <td width="10%">AE</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>     
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
  </tr>   
  <tr STYLE="font-size:10pt">
     <td width="10%">Wunden</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>     
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
     <td>&#160;</td>
  </tr>   
</table>

<!-- *************************************************************************** -->
<!-- *                               Seite 2                                     -->
<!-- *************************************************************************** -->

<h3 style="page-break-before:always">Talente, Sprachen und Gaben für <xsl:apply-templates select="Name" /></h3>

<table cols="8" cellpadding="2" cellspacing="0" border="1" width="640">
  <tr>
    <td width="12.5%">MU: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Mut']/Wert" /></b></td>
    <td width="12.5%">KL: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Klugheit']/Wert" /></b></td>
    <td width="12.5%">IN: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Intuition']/Wert" /></b></td>
    <td width="12.5%">CH: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Charisma']/Wert" /></b></td>
    <td width="12.5%">FF: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Fingerfertigkeit']/Wert" /></b></td>
    <td width="12.5%">GE: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Gewandtheit']/Wert" /></b></td>
    <td width="12.5%">KO: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Konstitution']/Wert" /></b></td>
    <td width="12.5%">KK: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Körperkraft']/Wert" /></b></td>
  </tr>
</table>

<!-- *************************************************************************** -->
<!-- *                      Talente, Sprachen, Gaben                             -->
<!-- *************************************************************************** -->

<table cols="2" cellpadding="1" cellspacing="0" width="640" border="0">
  <tr>
    <td valign="top" width="50%">
      <xsl:apply-templates select="Talente/Talentgruppe[@ID='Körperliche Talente']" />
      <xsl:apply-templates select="Talente/Talentgruppe[@ID='Gesellschaftliche Talente']" />
      <xsl:apply-templates select="Talente/Talentgruppe[@ID='Natur-Talente']" />
    </td>
    <td valign="top" width=",*">
      <xsl:apply-templates select="Talente/Talentgruppe[@ID='Wissenstalente']" />
      <xsl:apply-templates select="Talente/Talentgruppe[@ID='Handwerkliche Talente']" />
      <xsl:apply-templates select="Sprachen" />
      <xsl:apply-templates select="Schriften" />
      <xsl:apply-templates select="Talente/Talentgruppe[@ID='Gaben']" />
    </td>
  </tr>
</table>

<!-- *************************************************************************** -->
<!-- *                      Seite 3: Zauberfertigkeiten                          -->
<!-- *************************************************************************** -->

<xsl:if test="$AE != 0">
<h3 style="page-break-before:always">Zauberfertigkeiten für <xsl:apply-templates select="Name" /></h3>

<table cols="8" cellpadding="2" cellspacing="0" border="1" width="640">
  <tr>
    <td width="12.5%">MU: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Mut']/Wert" /></b></td>
    <td width="12.5%">KL: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Klugheit']/Wert" /></b></td>
    <td width="12.5%">IN: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Intuition']/Wert" /></b></td>
    <td width="12.5%">CH: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Charisma']/Wert" /></b></td>
    <td width="12.5%">FF: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Fingerfertigkeit']/Wert" /></b></td>
    <td width="12.5%">GE: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Gewandtheit']/Wert" /></b></td>
    <td width="12.5%">KO: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Konstitution']/Wert" /></b></td>
    <td width="12.5%">KK: <b><xsl:value-of select="Eigenschaften/Eigenschaft[@ID='Körperkraft']/Wert" /></b></td>
  </tr>
</table>

<table cols="8" cellpadding="3" cellspacing="0" border="1" width="640">
    <tr valign="center" align="left">
       <th width="22%">Zaubername</th>
       <th width="15%">Probe</th>
       <th width="5%">ZfW</th>
       <th width="8.5%">Z-Dauer</th>
       <th width="8%">Kosten</th>
       <th width="6%">Reichw.</th>
       <th width="10%">W-Dauer</th>
       <th width=",*">Bemerkung</th>
    </tr>
    <tr>
       <td>&#160;</td>
       <td>&#160;</td>
       <td>&#160;</td>
       <td>&#160;</td>
       <td>&#160;</td>
       <td>&#160;</td>
       <td>&#160;</td>
       <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
           <td>&#160;</td>
    </tr>             
    <tr>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
        </tr>             
        <tr>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
        </tr>             
        <tr>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
               <td>&#160;</td>
    </tr>             
<!--
    <xsl:for-each select="zauber">
    <xsl:sort select="begriff" order="ascending" data-type="text" />
    <tr STYLE="font-size: 10pt">
       <td><xsl:value-of select="begriff" /></td>
       <td><xsl:value-of select="zprobe" /></td>
       <td><xsl:value-of select="zfw" /></td>
       <td><xsl:value-of select="zdauer" /></td>
       <td><xsl:value-of select="zkosten" /></td>
       <td><xsl:value-of select="zreichweite" /></td>
       <td><xsl:value-of select="zwirkdauer" /></td>
       <td><xsl:value-of select="zbemerkung" /></td>
   </tr>
   </xsl:for-each>
-->

</table>
 <table border="1" width="640">
 <tr cols="2" valign="top">
    <td width="10%">Astral-Energie</td>
    <td><font size="1">(MU+IN+CH) / 2 + Modifikationen + Vor/Nachteil + Sonderfertigk. + Meditation + Zukauf = Max    -    pAsP = Aktuell
    </font>
    </td>
 </tr>
 </table>
 <table border="1" width="640">
 <tr cols="2" valign="top">
    <td width="10%">Magie-Resistenz</td>
    <td><font size="1">(MU+KL+KO) / 5 + Modifikationen + Vor/Nachteil + Sonderfertigk. = Aktuell
    </font>
    </td>
 </tr>
 </table>
</xsl:if>

<!-- *************************************************************************** -->
<!-- *                      Seite 4: Besitztümer                                 -->
<!-- *************************************************************************** -->

<h3 style="page-break-before:always">Güter und Besitztümer von <xsl:apply-templates select="Name" /></h3>
<b>Barvermögen</b>
<table cols="12" cellpadding="2" cellspacing ="0" border="1" width="640">
   <tr>
      <td width="12%">Dukaten</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
   </tr>
   <tr>
      <td width="12%">Silbertaler</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
   </tr>
   <tr>
      <td width="12%">Heller</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
   </tr>
   <tr>
      <td width="12%">Kreuzer</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
      <td>&#160;</td>
   </tr>
</table>   
<br />

<b>Ausrüstungsgegenstände</b>
<table cols="1" cellpadding="3" cellspacing="0" width="640" height="300" border="1">
   <tr>
      <td>&#160;</td>
   </tr>
</table>
<br />

<b>Notizen</b>
<table cols="1" cellpadding="3" cellspacing="0" width="640" height="400" border="1">
   <tr>
      <td>&#160;</td>
   </tr>
</table>

</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         T E M P L A T E S                                 -->
<!-- *************************************************************************** -->

<xsl:template match="Name">
<font face="Comic Sans MS" size="4"><xsl:value-of select="." /></font>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Eigenschaften                           -->
<!-- *************************************************************************** -->

<xsl:template match="Eigenschaften">
  <table cols="4" cellpadding="2" cellspacing="0" width="160" border="1">
     <xsl:apply-templates select="Eigenschaft" />
     <tr><td>Stufe</td><td>&#160;</td><td>&#160;</td><td>&#160;</td></tr>
     <tr><td>AP:</td><td colspan="3">&#160;</td></tr> 
   </table>  
</xsl:template>

<xsl:template match="Eigenschaft">
        <tr>
          <td width="40%">
             <xsl:variable name="eigen" select="@ID"></xsl:variable>
             <xsl:if test="$eigen='Mut'">MU</xsl:if>
             <xsl:if test="$eigen='Klugheit'">KL</xsl:if>
             <xsl:if test="$eigen='Intuition'">IN</xsl:if>
             <xsl:if test="$eigen='Charisma'">CH</xsl:if>
             <xsl:if test="$eigen='Fingerfertigkeit'">FF</xsl:if>
             <xsl:if test="$eigen='Gewandtheit'">GE</xsl:if>
             <xsl:if test="$eigen='Konstitution'">KO</xsl:if>
             <xsl:if test="$eigen='Körperkraft'">KK</xsl:if>
             </td>
          <td width="20%"><xsl:value-of select="Wert" /></td>
          <td width="20%">&#160;</td>
          <td width=",*%">&#160;</td>
        </tr>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Abgeleitete Werte                       -->
<!-- *************************************************************************** -->

<xsl:template match="AbgeleiteteWerte">
  <table cols="4" cellpadding="2" cellspacing="0" width="160" border="1">
    <xsl:apply-templates select="AbgeleiteterWert" />
         <tr><td>AP-G:</td><td colspan="3">&#160;</td></tr> 
  </table>
</xsl:template>

<xsl:template match="AbgeleiteterWert">
  <tr>
    <td width="40%">
      <xsl:variable name="temp" select="@ID"></xsl:variable>
      <xsl:if test="$temp='Lebensenergie'">LE</xsl:if>
      <xsl:if test="$temp='Ausdauer'">AU</xsl:if>
      <xsl:if test="$temp='Magieresistenz'">MR</xsl:if>
      <xsl:if test="$temp='Astralenergie'">AE</xsl:if>
      <xsl:if test="$temp='Sozialstatus'">SO</xsl:if>
      <xsl:if test="$temp='AttackeBasis'">B-AT</xsl:if>
      <xsl:if test="$temp='ParadeBasis'">B-PA</xsl:if>
      <xsl:if test="$temp='FernkampfBasis'">B-FK</xsl:if>
      <xsl:if test="$temp='InitiativeBasis'">B-IN</xsl:if>
    </td>
    <td align="right" width="20%"><xsl:value-of select="Wert" /></td>
    <td width="20%">&#160;</td>
    <td width=",*">&#160;</td>
  </tr>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Kampftechniken                          -->
<!-- *************************************************************************** -->

<xsl:template match="Kampftechniken">
  <table cols="5" cellpadding="1" cellspacing="0" border="1" width="320">
    <tr>
      <th colspan="2">Kampftechnik</th>
      <th>AT-PA</th>
      <th colspan="2">TaW</th>
    </tr>
    <xsl:apply-templates select="Kampftechnik" />
  </table>
</xsl:template>

<xsl:template match="Kampftechnik">
  <tr STYLE="font-size: 10pt">
    <td width="53%"><xsl:variable name="idid" select="@ID" /><xsl:value-of select="$idid" /></td>
    <xsl:variable name="idid" select="@ID" />
    <td align="center" width="14%">
       <xsl:value-of select="document('data.xdi')/XDIML/Inhalt/Kampftechniken/Kampfart/Kampftechnik[@ID=$idid]/eBE" /></td>
    <td align="center" width="17%"><xsl:value-of select="Attacke" /> / <xsl:value-of select="Parade" /></td>
    <td align="center" width="8%"><xsl:value-of select="Wert" /></td>
    <td width=",*">&#160;</td>
  </tr>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Charakteristika                         -->
<!-- *************************************************************************** -->

<xsl:template match="Charakteristika">
  <xsl:variable name="bis" select="count(Charakteristikum)" />
  <table cols="2" cellpadding="2" cellspacing="0" border="1">
    <xsl:if test="$bis &gt; 0"> 
    <tr STYLE="font-size:10pt">
      <td width="50%"><xsl:value-of select="Charakteristikum[1]/@ID" />&#160;<xsl:value-of select="Charakteristikum[1]/Stufe" /></td>
      <td width=",*"><xsl:value-of select="Charakteristikum[2]/@ID" />&#160;<xsl:value-of select="Charakteristikum[2]/Stufe" /></td>
    </tr>  
    </xsl:if>  
    <xsl:if test="$bis &gt; 2"> 
    <tr STYLE="font-size:10pt">
      <td width="50%"><xsl:value-of select="Charakteristikum[3]/@ID" />&#160;<xsl:value-of select="Charakteristikum[3]/Stufe" /></td>
      <td width=",*"><xsl:value-of select="Charakteristikum[4]/@ID" />&#160;<xsl:value-of select="Charakteristikum[4]/Stufe" /></td>
    </tr>  
    </xsl:if>
    <xsl:if test="$bis &gt; 4"> 
      <tr STYLE="font-size:10pt">
        <td width="50%"><xsl:value-of select="Charakteristikum[5]/@ID" />&#160;<xsl:value-of select="Charakteristikum[5]/Stufe" /></td>
        <td width=",*"><xsl:value-of select="Charakteristikum[6]/@ID" />&#160;<xsl:value-of select="Charakteristikum[6]/Stufe" /></td>
      </tr>  
    </xsl:if>  
    <xsl:if test="$bis &gt; 6"> 
      <tr STYLE="font-size:10pt">
        <td width="50%"><xsl:value-of select="Charakteristikum[7]/@ID" />&#160;<xsl:value-of select="Charakteristikum[7]/Stufe" /></td>
        <td width=",*"><xsl:value-of select="Charakteristikum[8]/@ID" />&#160;<xsl:value-of select="Charakteristikum[8]/Stufe" /></td>
      </tr>  
    </xsl:if>  
  </table>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Sonderfertigkeiten                      -->
<!-- *************************************************************************** -->

<xsl:template match="Sonderfertigkeiten">
  <xsl:variable name="bis" select="count(Sonderfertigkeit)" />
  <table cols="2" cellpadding="2" cellspacing="0" border="1">
    <xsl:if test="$bis &gt; 0"> 
    <tr STYLE="font-size:10pt">
      <td width="50%"><xsl:value-of select="Sonderfertigkeit[1]/@ID" />&#160;</td>
      <td width=",*"><xsl:value-of select="Sonderfertigkeit[2]/@ID" />&#160;</td>
    </tr>  
    </xsl:if>  
    <xsl:if test="$bis &gt; 2"> 
    <tr STYLE="font-size:10pt">
      <td width="50%"><xsl:value-of select="Sonderfertigkeit[3]/@ID" />&#160;</td>
      <td width=",*"><xsl:value-of select="Sonderfertigkeit[4]/@ID" />&#160;</td>
    </tr>  
    </xsl:if>
    <xsl:if test="$bis &gt; 4"> 
      <tr STYLE="font-size:10pt">
        <td width="50%"><xsl:value-of select="Sonderfertigkeit[5]/@ID" />&#160;</td>
        <td width=",*"><xsl:value-of select="Sonderfertigkeit[6]/@ID" />&#160;</td>
      </tr>  
    </xsl:if>  
    <xsl:if test="$bis &gt; 6"> 
      <tr STYLE="font-size:10pt">
        <td width="50%"><xsl:value-of select="Sonderfertigkeit[7]/@ID" />&#160;</td>
        <td width=",*"><xsl:value-of select="Sonderfertigkeit[8]/@ID" />&#160;</td>
      </tr>  
    </xsl:if>  
  </table>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Körperliche Talente                     -->
<!-- *************************************************************************** -->

<xsl:template match="Talente/Talentgruppe[@ID='Körperliche Talente']">
<table cols="4" cellpadding="1" cellspacing="0" border="1" width="320">
  <tr>
    <th colspan="2" width="84%">Körperliche Talente (D)</th>
    <th colspan="2" width=",*">TaW</th>
  </tr>
  <xsl:for-each select="Talent">
  <tr STYLE="font-size:10pt">
     <td>
        <table cols="2" border="0" cellpadding="1" cellspacing="0">
           <tr STYLE="font-size:10pt">
              <td align="left" width="146">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="$idid" />
              </td>   
              <td align="right" width="44">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="document('data.xdi')/XDIML/Inhalt/Talente/Talentgruppe[@ID='Körperliche Talente']/Talent[@ID=$idid]/Probe" />
              </td>
           </tr>
        </table>  
     </td>
     <xsl:variable name="idid" select="@ID" />
     <td width="36"><xsl:value-of select="document('data.xdi')/XDIML/Inhalt/Talente/Talentgruppe[@ID='Körperliche Talente']/Talent[@ID=$idid]/eBE" /></td>
     <td align="center" width="25"><xsl:value-of select="Wert" /></td>
     <td width="25">&#160;</td>
  </tr>  
  </xsl:for-each>    
</table>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Gesellschaftliche Talente               -->
<!-- *************************************************************************** -->

<xsl:template match="Talente/Talentgruppe[@ID='Gesellschaftliche Talente']">
<table cols="4" cellpadding="1" cellspacing="0" border="1" width="320">
  <tr>
    <th colspan="2" width="84%">Gesellschaftliche Talente (B)</th>
    <th colspan="2" width=",*">TaW</th>
  </tr>
  <xsl:for-each select="Talent">
  <tr STYLE="font-size:10pt">
     <td>
        <table cols="2" border="0" cellpadding="1" cellspacing="0">
           <tr STYLE="font-size:10pt">
              <td align="left" width="146">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="$idid" />
              </td>   
              <td align="right" width="44">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="document('data.xdi')/XDIML/Inhalt/Talente/Talentgruppe[@ID='Gesellschaftliche Talente']/Talent[@ID=$idid]/Probe" />
              </td>
           </tr>
        </table>  
     </td>
     <xsl:variable name="idid" select="@ID" />
     <td width="36"><xsl:value-of select="document('data.xdi')/XDIML/Inhalt/Talente/Talentgruppe[@ID='Gesellschaftliche Talente']/Talent[@ID=$idid]/eBE" />
     </td>
     <td align="center" width="25"><xsl:value-of select="Wert" /></td>
     <td width="25">&#160;</td>
  </tr>  
  </xsl:for-each>    
</table>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Natur-Talente                           -->
<!-- *************************************************************************** -->

<xsl:template match="Talente/Talentgruppe[@ID='Natur-Talente']">
<table cols="4" cellpadding="1" cellspacing="0" border="1" width="320">
  <tr>
    <th colspan="2" width="84%">Natur-Talente (B)</th>
    <th colspan="2" width=",*">TaW</th>
  </tr>
  <xsl:for-each select="Talent">
  <tr STYLE="font-size:10pt">
     <td>
        <table cols="2" border="0" cellpadding="1" cellspacing="0">
           <tr STYLE="font-size:10pt">
              <td align="left" width="146">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="$idid" />
              </td>   
              <td align="right" width="44">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="document('data.xdi')/XDIML/Inhalt/Talente/Talentgruppe[@ID='Natur-Talente']/Talent[@ID=$idid]/Probe" />
              </td>
           </tr>
        </table>  
     </td>
     <xsl:variable name="idid" select="@ID" />
     <td width="36"><xsl:value-of select="document('data.xdi')/XDIML/Inhalt/Talente/Talentgruppe[@ID='Natur-Talente']/Talent[@ID=$idid]/eBE" /></td>
     <td align="center" width="25"><xsl:value-of select="Wert" /></td>
     <td width="25">&#160;</td>
  </tr>  
  </xsl:for-each>    
</table>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Wissenstalente                           -->
<!-- *************************************************************************** -->

<xsl:template match="Talente/Talentgruppe[@ID='Wissenstalente']">
<table cols="3" cellpadding="1" cellspacing="0" border="1" width="320">
  <tr>
    <th width="84%">Wissenstalente (B)</th>
    <th colspan="2" width=",*">TaW</th>
  </tr>  
  <xsl:for-each select="Talent">
  <tr STYLE="font-size:10pt">
     <td>
        <table cols="2" border="0" cellpadding="1" cellspacing="0">
           <tr STYLE="font-size:10pt">
              <td align="left" width="196">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="$idid" />
              </td>   
              <td align="right" width="40">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="document('data.xdi')/XDIML/Inhalt/Talente/Talentgruppe[@ID='Wissenstalente']/Talent[@ID=$idid]/Probe" />
              </td>
           </tr>
        </table>  
     </td>
     <td align="center" width="25"><xsl:value-of select="Wert" /></td>
     <td width="25">&#160;</td>
  </tr>  
  </xsl:for-each>    
</table>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Handwerkliche Talente                             -->
<!-- *************************************************************************** -->

<xsl:template match="Talente/Talentgruppe[@ID='Handwerkliche Talente']">
<table cols="3" cellpadding="1" cellspacing="0" border="1" width="320">
  <tr>
    <th width="84%">Handwerkliche Talente (B)</th>
    <th colspan="2" width=",*">TaW</th>
  </tr>
  <xsl:for-each select="Talent">
  <tr STYLE="font-size:10pt">
     <td>
        <table cols="2" border="0" cellpadding="1" cellspacing="0">
           <tr STYLE="font-size:10pt">
              <td align="left" width="196">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="$idid" />
              </td>   
              <td align="right" width="40">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="document('data.xdi')/XDIML/Inhalt/Talente/Talentgruppe[@ID='Handwerkliche Talente']/Talent[@ID=$idid]/Probe" />
              </td>
           </tr>
        </table>  
     </td>
     <td align="center" width="25"><xsl:value-of select="Wert" /></td>
     <td width="25">&#160;</td>
  </tr>  
  </xsl:for-each>    
</table>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Gaben                                             -->
<!-- *************************************************************************** -->

<xsl:template match="Talente/Talentgruppe[@ID='Gaben']">
<table cols="3" cellpadding="1" cellspacing="0" border="1" width="320">
  <tr>
    <th width="84%">Gaben (G)</th>
    <th colspan="2" width=",*">TaW</th>
  </tr>
  <xsl:for-each select="Talent">
  <tr STYLE="font-size:10pt">
     <td>
        <table cols="2" border="0" cellpadding="1" cellspacing="0">
           <tr STYLE="font-size:10pt">
              <td align="left" width="196">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="$idid" />
              </td>   
              <td align="right" width="40">
                 <xsl:variable name="idid" select="@ID" />
                 <xsl:value-of select="document('data.xdi')/XDIML/Inhalt/Talente/Talentgruppe[@ID='Gaben']/Talent[@ID=$idid]/Probe" />
              </td>
           </tr>
        </table>  
     </td>
     <td align="center" width="25"><xsl:value-of select="Wert" /></td>
     <td width="25">&#160;</td>
  </tr>  
  </xsl:for-each>    
</table>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Sprachen                                -->
<!-- *************************************************************************** -->

<xsl:template match="Sprachen">
<table cols="3" cellpadding="2" cellspacing="0" border="1" width="320">
    <tr>
      <th width="84%">Sprachen (A)</th>
      <th colspan="2" width=",*">TaW</th>
    </tr>
    <xsl:apply-templates select="Sprache" />
</table>
</xsl:template>

<xsl:template match="Sprache">
  <tr STYLE="font-size: 10pt">
    <td><xsl:value-of select="@ID" /></td>
    <td align="center" width="25"><xsl:value-of select="Wert" /></td>
    <td width="25">&#160;</td>
  </tr>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Schriften                               -->
<!-- *************************************************************************** -->

<xsl:template match="Schriften">
  <table cols="3" cellpadding="2" cellspacing="0" border="1" width="320">
    <tr>
      <th width="84%">Schriften (A)</th>
      <th colspan="2" width=",*">TaW</th>
    </tr>
    <xsl:apply-templates select="Schrift" />
  </table>
</xsl:template>

<xsl:template match="Schrift">
  <tr STYLE="font-size: 10pt">
    <td><xsl:value-of select="@ID" /></td>
    <td align="center" width="25"><xsl:value-of select="Wert" /></td>
    <td width="25">&#160;</td>
  </tr>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Nahkampfwaffen                          -->
<!-- *************************************************************************** -->

<xsl:template match="Waffen/Waffengruppe[@ID='Nahkampfwaffen']">
<xsl:for-each select="Waffe">
  <tr STYLE="font-size:11pt">
    <td><xsl:value-of select="@ID" /></td>
    <td><xsl:value-of select="TypeBE" /></td>
    <td><xsl:value-of select="DKlasse" /></td>
    <td><xsl:value-of select="TP" /></td>
    <td><xsl:value-of select="TPKK" /></td>
    <td><xsl:value-of select="INI" /></td>
    <td><xsl:value-of select="WM" /></td>
    <td><xsl:value-of select="AT" /></td>
    <td><xsl:value-of select="PA" /></td>
    <td><xsl:value-of select="TPeff" /></td>
    <td><xsl:value-of select="BF" /></td>
  </tr>
</xsl:for-each>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Fernkampfwaffen                         -->
<!-- *************************************************************************** -->

<xsl:template match="Waffen/Waffengruppe[@ID='Fernkampfwaffen']">
<xsl:for-each select="Waffe">
  <tr STYLE="font-size:11pt">
    <td><xsl:value-of select="@ID" /></td>
    <td><xsl:value-of select="TypeBE" /></td>
    <td><xsl:value-of select="TP" /></td>
    <td><xsl:value-of select="Entfernungen" /></td>
    <td><xsl:value-of select="TPEntfernung" /></td>
    <td><xsl:value-of select="FK" /></td>
    <td><xsl:value-of select="Geschosse" /></td>
  </tr>
</xsl:for-each>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Rüstungen                               -->
<!-- *************************************************************************** -->

<xsl:template match="Ruestungen">
<xsl:for-each select="Ruestungsstueck">
   <tr STYLE="font-size:11pt">
      <td><xsl:value-of select="@ID" /></td>
      <td><xsl:value-of select="RS" /></td>
      <td><xsl:value-of select="BE" /></td>
   </tr>
</xsl:for-each>
</xsl:template>

<!-- *************************************************************************** -->
<!-- *                         Template: Schilde                                 -->
<!-- *************************************************************************** -->

<xsl:template match="Schilde">
<xsl:for-each select="Schild">
   <tr STYLE="font-size:11pt">
      <td><xsl:value-of select="@ID" /></td>
      <td><xsl:value-of select="INI" /></td>
      <td><xsl:value-of select="WM" /></td>
   </tr>
</xsl:for-each>
</xsl:template>

</xsl:stylesheet>
