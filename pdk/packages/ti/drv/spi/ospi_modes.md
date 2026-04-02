@ingroup DRV_SPI_MODULE
@defgroup DRV_SPI_MODES_MODULE SPI Driver Modes
@{

@if j721e
<table>
<tr><th colspan="100%">MICRON MT35 NAND Flash
<tr><th>                    <th>                <th colspan="2">OSPI Cache Enabled  <th colspan="2">OSPI Cache Disabled <th> 
<tr><th>Mode                <th>Feature         <th>Without PHY <th>PHY+Pipeline    <th>Without PHY <th>PHY+Pipeline    <th>Comments
<tr><td rowspan="5">DAC     <td>CPU Read        <td>YES         <td>YES             <td>YES         <td>No              <td> 
<tr>                        <td>CPU Write       <td>YES         <td>YES             <td>YES         <td>No              <td> 
<tr>                        <td>DMA Read        <td>YES         <td>YES             <td>YES         <td>No              <td> 
<tr>                        <td>DMA Write       <td>YES         <td>YES             <td>YES         <td>No              <td> 
<tr>                        <td>Frequency (MHz) <td>4           <td>166             <td>4           <td>166             <td> 
<tr><td rowspan="3">INDAC   <td>CPU Read        <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>CPU Write       <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>4           <td>NA              <td>NA          <td>NA              <td> 
<tr><td rowspan="4">SBL     <td>CPU             <td>Not Tested  <td>Not Tested      <td>YES         <td>YES             <td> 
<tr>                        <td>DMA             <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
<tr>                        <td>XIP             <td>Not Tested  <td>Not Tested      <td>YES         <td>Not Tested      <td> 
<tr>                        <td>Linux Boot      <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
</table>
@endif

@if j7200
<table>
<tr><th colspan="100%">Cypress S28 NOR Flash
<tr><th>                    <th>                <th colspan="2">OSPI Cache Enabled  <th colspan="2">OSPI Cache Disabled <th> 
<tr><th>Mode                <th>Feature         <th>Without PHY <th>PHY+Pipeline    <th>Without PHY <th>PHY+Pipeline    <th>Comments
<tr><td rowspan="5">DAC     <td>CPU Read        <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>CPU Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>DMA Read        <td>YES         <td>YES             <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>DMA Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>33          <td>166             <td>33          <td>166             <td> 
<tr><td rowspan="3">INDAC   <td>CPU Read        <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>CPU Write       <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>33          <td>NA              <td>NA          <td>NA              <td> 
<tr><td rowspan="4">SBL     <td>CPU             <td>Not Tested  <td>Not Tested      <td>YES         <td>NA              <td> 
<tr>                        <td>DMA             <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
<tr>                        <td>XIP             <td>Not Tested  <td>Not Tested      <td>YES         <td>Not Tested      <td> 
<tr>                        <td>Linux Boot      <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
</table>
@endif

@if j721s2
<table>
<tr><th colspan="100%">Cypress S28 NOR Flash
<tr><th>                    <th>                <th colspan="2">OSPI Cache Enabled  <th colspan="2">OSPI Cache Disabled <th> 
<tr><th>Mode                <th>Feature         <th>Without PHY <th>PHY+Pipeline    <th>Without PHY <th>PHY+Pipeline    <th>Comments
<tr><td rowspan="5">DAC     <td>CPU Read        <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>CPU Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>DMA Read        <td>YES         <td>YES             <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>DMA Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>33          <td>166             <td>33          <td>166             <td> 
<tr><td rowspan="3">INDAC   <td>CPU Read        <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>CPU Write       <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>33          <td>NA              <td>NA          <td>NA              <td> 
<tr><td rowspan="4">SBL     <td>CPU             <td>Not Tested  <td>Not Tested      <td>YES         <td>NA              <td> 
<tr>                        <td>DMA             <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
<tr>                        <td>XIP             <td>Not Tested  <td>Not Tested      <td>YES         <td>Not Tested      <td> 
<tr>                        <td>Linux Boot      <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
</table>

<table>
<tr><th colspan="100%">Winbond W35N NAND Flash
<tr><th>                    <th>                <th colspan="2">OSPI Cache Enabled  <th colspan="2">OSPI Cache Disabled <th> 
<tr><th>Mode                <th>Feature         <th>Without PHY <th>PHY+Pipeline    <th>Without PHY <th>PHY+Pipeline    <th>Comments
<tr><td rowspan="5">DAC     <td>CPU Read        <td>YES         <td>Not Tested      <td>YES         <td>Not Tested      <td> 
<tr>                        <td>CPU Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>DMA Read        <td>Not Tested  <td>YES             <td>Not Tested  <td>YES             <td> 
<tr>                        <td>DMA Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>50          <td>166             <td>50          <td>166             <td> 
<tr><td rowspan="3">INDAC   <td>CPU Read        <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>CPU Write       <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>22          <td>NA              <td>NA          <td>NA              <td> 
<tr><td rowspan="4">SBL     <td>CPU             <td>Not Tested  <td>Not Tested      <td>YES         <td>NA              <td> 
<tr>                        <td>DMA             <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>XIP             <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>Linux Boot      <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>Not Tested      <td> 
</table>
@endif

@if j784s4
<table>
<tr><th colspan="100%">Cypress S28 NOR Flash
<tr><th>                    <th>                <th colspan="2">OSPI Cache Enabled  <th colspan="2">OSPI Cache Disabled <th> 
<tr><th>Mode                <th>Feature         <th>Without PHY <th>PHY+Pipeline    <th>Without PHY <th>PHY+Pipeline    <th>Comments
<tr><td rowspan="5">DAC     <td>CPU Read        <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>CPU Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>DMA Read        <td>YES         <td>YES             <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>DMA Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>33          <td>166             <td>33          <td>166             <td> 
<tr><td rowspan="3">INDAC   <td>CPU Read        <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>CPU Write       <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>33          <td>NA              <td>NA          <td>NA              <td> 
<tr><td rowspan="4">SBL     <td>CPU             <td>Not Tested  <td>Not Tested      <td>YES         <td>NA              <td> 
<tr>                        <td>DMA             <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
<tr>                        <td>XIP             <td>Not Tested  <td>Not Tested      <td>YES         <td>Not Tested      <td> 
<tr>                        <td>Linux Boot      <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
</table>

<table>
<tr><th colspan="100%">Winbond W35N NAND Flash
<tr><th>                    <th>                <th colspan="2">OSPI Cache Enabled  <th colspan="2">OSPI Cache Disabled <th> 
<tr><th>Mode                <th>Feature         <th>Without PHY <th>PHY+Pipeline    <th>Without PHY <th>PHY+Pipeline    <th>Comments
<tr><td rowspan="5">DAC     <td>CPU Read        <td>YES         <td>Not Tested      <td>YES         <td>Not Tested      <td> 
<tr>                        <td>CPU Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>DMA Read        <td>Not Tested  <td>YES             <td>Not Tested  <td>YES             <td> 
<tr>                        <td>DMA Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>50          <td>166             <td>50          <td>166             <td> 
<tr><td rowspan="3">INDAC   <td>CPU Read        <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>CPU Write       <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>22          <td>NA              <td>NA          <td>NA              <td> 
<tr><td rowspan="4">SBL     <td>CPU             <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>NA              <td> 
<tr>                        <td>DMA             <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>XIP             <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>Linux Boot      <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>Not Tested      <td> 
</table>
@endif

@if am65xx
<table>
<tr><th>                    <th>                <th colspan="2">OSPI Cache Enabled  <th colspan="2">OSPI Cache Disabled <th> 
<tr><th>Mode                <th>Feature         <th>Without PHY <th>PHY+Pipeline    <th>Without PHY <th>PHY+Pipeline    <th>Comments
<tr><td rowspan="5">DAC     <td>CPU Read        <td>Not Tested  <td>YES             <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>CPU Write       <td>Not Tested  <td>YES             <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>DMA Read        <td>Not Tested  <td>YES             <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>DMA Write       <td>Not Tested  <td>YES             <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>Frequency (MHz) <td>4           <td>166             <td>4           <td>166             <td> 
<tr><td rowspan="3">INDAC   <td>CPU Read        <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>CPU Write       <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>33          <td>NA              <td>NA          <td>NA              <td> 
<tr><td rowspan="4">SBL     <td>CPU             <td>Not Tested  <td>Not Tested      <td>YES         <td>YES             <td> 
<tr>                        <td>DMA             <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
<tr>                        <td>XIP             <td>Not Tested  <td>Not Tested      <td>YES         <td>Not Tested      <td> 
<tr>                        <td>Linux Boot      <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
</table>
@endif

@if am64x
<table>
<tr><th>                    <th>                <th colspan="2">OSPI Cache Enabled  <th colspan="2">OSPI Cache Disabled <th> 
<tr><th>Mode                <th>Feature         <th>Without PHY <th>PHY+Pipeline    <th>Without PHY <th>PHY+Pipeline    <th>Comments
<tr><td rowspan="5">DAC     <td>CPU Read        <td>YES         <td>YES             <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>CPU Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>DMA Read        <td>YES         <td>YES             <td>Not Tested  <td>Not Tested      <td> 
<tr>                        <td>DMA Write       <td>NA          <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>33          <td>166             <td>33          <td>166             <td> 
<tr><td rowspan="3">INDAC   <td>CPU Read        <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>CPU Write       <td>YES         <td>NA              <td>NA          <td>NA              <td> 
<tr>                        <td>Frequency (MHz) <td>33          <td>NA              <td>NA          <td>NA              <td> 
<tr><td rowspan="4">SBL     <td>CPU             <td>Not Tested  <td>Not Tested      <td>YES         <td>NA              <td> 
<tr>                        <td>DMA             <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
<tr>                        <td>XIP             <td>Not Tested  <td>Not Tested      <td>YES         <td>Not Tested      <td> 
<tr>                        <td>Linux Boot      <td>Not Tested  <td>Not Tested      <td>Not Tested  <td>YES             <td> 
</table>
@endif

@}