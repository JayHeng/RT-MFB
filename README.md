# imxrt-multi-flash-boot-solution
一种灵活的i.MXRT下多串行NOR Flash型号选择的量产方案

![](/implementation_note.PNG)

### xSPI外设特性

<table><tbody>
    <tr>
        <th rowspan="2">芯片系列</th>
        <th colspan="3">RXCLKSRC</th>
    </tr>
    <tr>
        <td>0x0 Internal Loopback</td>
        <td>0x1 Loopback from DQS</td>
        <td>0x3 Ext DQS</td>
    </tr>
    <tr>
        <td>RT10xx, RT11xx<br>
            FlexSPI x8</td>
        <td>SDR 60MHz<br>
            DDR 30MHz</td>
        <td>SDR 133MHz<br>
            DDR 66MHz</td>
        <td>SDR 166MHz<br>
            DDR 166MHz</td>
    </tr>
    <tr>
        <td>RT5xx, RT6xx<br>
            FlexSPI x8</td>
        <td>SDR 60MHz<br>
            DDR 30MHz</td>
        <td>SDR 116MHz<br>
            DDR 57MHz</td>
        <td>SDR 200MHz<br>
            DDR 200MHz</td>
    </tr>
    <tr>
        <td>RT7xx<br>
            xSPI x16</td>
        <td>SDR 60MHz<br>
            DDR 30MHz</td>
        <td>SDR 116MHz<br>
            DDR 57MHz</td>
        <td>SDR 200MHz<br>
            DDR 250MHz</td>
    </tr>
</table>

### 已测试Flash型号表

下列测试结果基于 RT1180-FLEXSPI-MEM-BB&DC_Rev.B（默认板卡设置）FlexSPI1 PortA MUX1 （GPIO_B2_XX），这组 Pin 也是 RT1180-EVK 默认选择。

```text
FlexSPI1 pin multiplex options performance specifications:
1. GPIO_B1_XX and GPIO_B2_XX support the maximum 200 MHz operation frequency;
2. GPIO_SD_B2_XX only supports the maximum 166 MHz operation frequency.

FlexSPI2 pin multiplex options performance specifications:
1. GPIO_EMC_B1_XX supports the maximum 166 MHz operation frequency;
2. GPIO_AON_XX only supports the maximum 104 MHz operation frequency
```

```text
- 确保BB-[J82-J90]-2&3, J109-Open
- 测试仅改动 BB-J68&J16 以设置 Flash_VCC: 3.3V/1.8V/1.2V
  -- J68.1-2 3.3V
  -- J68.2-3 1.8V
  -- J16.1-2 1.2V
- 测试仅改动 BB-J11 以切换 QUAD/OCTAL Package
  -- J11.1-2 OCTAL
  -- J11.2-3 QUAD
```

<table><tbody>
    <tr>
        <th>厂商</th>
        <th>型号</th>
        <th>电压</th>
        <th>速度</th>
        <th>测试项目</th>
        <th>测试结果</th>
    </tr>
    <tr>
        <td rowspan="11">华邦<br>
                         Winbond<br>
        <td>W25Q40CLNIG</td>
        <td>2.3-3.6V</td>
        <td>104M (x4)</td>
        <td>S104Mx4</td>
        <td>TBD，封装不便测试</td>
    </tr>
    <tr>
        <td>W25Q16JVSIQ</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td>S120Mx4<br>
            S133Mx4</td>
        <td>通过<br>
            回读校验失败</td>
    </tr>
    <tr>
        <td>W25Q32JVSIQ</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q64NESIG</td>
        <td>1.14V-1.26V</td>
        <td>84M (x4)</td>
        <td>S84Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q12NESSIG</td>
        <td>1.14V-1.26V</td>
        <td>104M (x4)</td>
        <td>S84Mx4<br>
            S104Mx4</td>
        <td>通过<br>
            通过（100% DRV）</td>
    </tr>
    <tr>
        <td>W25Q64JWSJQ</td>
        <td>1.7-1.95V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q128JWSQ/M</td>
        <td>1.7-1.95V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q256JWBAQ</td>
        <td>1.7-1.95V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25QQ512NWSIQ/M</td>
        <td>1.7-1.95V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>W25M512JWBIQ</td>
        <td>1.7-1.95V</td>
        <td>104M (x4)</td>
        <td>S104Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W35T51NW</td>
        <td>1.7-2.0V</td>
        <td>200M (x8D)</td>
        <td>D100Mx8<br>
            D166Mx8<br>
            D200Mx8</td>
        <td>通过<br>
            回读校验失败<br>
            回读校验失败</td>
    </tr>
    <tr>
        <td rowspan="21">旺宏<br>
                         Macronix</td>
        <td>MX25V4035F</td>
        <td>2.3-3.6V</td>
        <td>104M (x4)</td>
        <td>S104Mx4</td>
        <td>TBD，封装不便测试</td>
    </tr>
    <tr>
        <td>MX25V8035F</td>
        <td>2.3-3.6V</td>
        <td>104M (x4)</td>
        <td>S104Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25V1635F</td>
        <td>2.3-3.6V</td>
        <td>80M (x4)</td>
        <td>S80Mx4</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下四线初始化挂起</td>
    </tr>
    <tr>
        <td>MX25U1632F</td>
        <td>1.65-2V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下QE设置失败</td>
    </tr>
    <tr>
        <td>MX25L3233F</td>
        <td>2.65-3.6V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25U3232F</td>
        <td>1.65-2V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下QE设置失败</td>
    </tr>
    <tr>
        <td>MX25U6435F</td>
        <td>1.65-2V</td>
        <td>104M (x4)</td>
        <td>S104Mx4</td>
        <td>QE设置失败</td>
    </tr>
    <tr>
        <td>MX25U6432F</td>
        <td>1.65-2V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下QE设置失败</td>
    </tr>
    <tr>
        <td>MX25L6433F</td>
        <td>2.65-3.6V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25U12832F</td>
        <td>1.65-2V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下QE设置失败</td>
    </tr>
    <tr>
        <td>MX25L12833F</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下QE设置失败</td>
    </tr>
    <tr>
        <td>MX25L25645G</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>QE设置失败</td>
    </tr>
    <tr>
        <td>MX25U25643G</td>
        <td>1.65-2V</td>
        <td>120M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>MX25U25645G</td>
        <td>1.65-2V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25UW6435G</td>
        <td>1.65-2V</td>
        <td>200M (x8D)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>MX25UW12845G</td>
        <td>1.65-2V</td>
        <td>200M (x8D)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>MX25UW25345G</td>
        <td>1.65-2V</td>
        <td>200M (x8D)</td>
        <td>D200Mx8</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25UW51345G</td>
        <td>1.65-2V</td>
        <td>200M (x8D)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>MX25UM51345G</td>
        <td>1.65-2V</td>
        <td>200M (x8D)</td>
        <td>D200Mx8</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25LM51245G</td>
        <td>2.7-3.6V</td>
        <td>200M (x8D)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>MX66UM1G45G</td>
        <td>1.65-2V</td>
        <td>200M (x8D)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td rowspan="18">兆易创新<br>
                         GigaDevice</td>
        <td>GD25LE32E</td>
        <td>1.65~2.0V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>GD25Q64CSIG</td>
        <td>2.7-3.6V</td>
        <td>120M (x4)</td>
        <td>S120Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD25LQ64E</td>
        <td>1.65~2.0V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>GD25LE64E</td>
        <td>1.65~2.0V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>GD25LB64ESIG</td>
        <td>1.65-2.0V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>回读校验失败</td>
    </tr>
    <tr>
        <td>GD25Q127CSIG</td>
        <td>2.7-3.6V</td>
        <td>104M (x4)</td>
        <td>S104Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD25B128ESIG</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD25LE128DSIG</td>
        <td>1.65-2.0V</td>
        <td>120M (x4)</td>
        <td>S120Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD25LQ128D</td>
        <td>1.65~2.0V</td>
        <td>120M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>GD25LF128E</td>
        <td>1.65~2.0V</td>
        <td>166M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>GD25LT256EB1R</td>
        <td>1.65-2.0V</td>
        <td>166M (x4)</td>
        <td>S166Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD25LB256D</td>
        <td>1.65-2.0V</td>
        <td>120M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>GD25LQ256D</td>
        <td>1.65-2.0V</td>
        <td>120M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>GD25LF255E</td>
        <td>1.65-2.0V</td>
        <td>166M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>GD25LT512</td>
        <td>1.65-2.0V</td>
        <td>200M (x4D)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>GD55B01G</td>
        <td>2.7~3.6V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>GD25X512MEB2R</td>
        <td>2.7-3.6V</td>
        <td>200M (x8)</td>
        <td>D166Mx8<br>
            D200Mx8</td>
        <td>通过<br>
            回读校验失败</td>
    </tr>
    <tr>
        <td>GD25LX512MEB1R</td>
        <td>1.65-2.0V</td>
        <td>200M (x8)</td>
        <td>D166Mx8<br>
            D200Mx8</td>
        <td>通过<br>
            回读校验失败</td>
    </tr>
    <tr>
        <td rowspan="11">芯成 ISSI</td>
        <td>IS25LP064A</td>
        <td>2.3-3.6V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>IS25WP064A</td>
        <td>1.65-1.95V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>IS25LP064D</td>
        <td>2.3-3.6V</td>
        <td>166M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>IS25WP064D</td>
        <td>1.65-1.95V</td>
        <td>166M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>IS25WP128</td>
        <td>1.65-1.95V</td>
        <td>133M (x4)</td>
        <td>S80Mx4<br>
            S133Mx4</td>
        <td>通过<br>
            回读校验失败(设置Dummy失败)</td>
    </tr>
    <tr>
        <td>IS25WP128F</td>
        <td>1.65-1.95V</td>
        <td>166M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>IS25LP128F</td>
        <td>2.3-3.6V</td>
        <td>166M (x4)</td>
        <td>S133Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>IS25LP512M</td>
        <td>2.3-3.6V</td>
        <td>133M (x4)</td>
        <td>S133Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>IS25LP01G</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>IS25WX256</td>
        <td>1.7-2.0V</td>
        <td>200M (x8)</td>
        <td>D166Mx8</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>IS25LX256</td>
        <td>2.7-3.6V</td>
        <td>200M (x8)</td>
        <td>D166Mx8</td>
        <td>通过</td>
    </tr>
    <tr>
        <td rowspan="5">镁光<br>
                        Micron</td>
        <td>MT25QU128_RW138</td>
        <td>1.7-2.0V</td>
        <td>166M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>MT25QL128_RW126</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>MT25QL256_RW162</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>MT35XU512_RW303</td>
        <td>1.7-2.0V</td>
        <td>200M (x8)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>MT35XU512_RW304</td>
        <td>1.7-2.0V</td>
        <td>200M (x8)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td rowspan="6">Renesas<br>
                        (Adesto)</td>
        <td>AT25QF641</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>AT25QL641</td>
        <td>1.7-2.0V</td>
        <td>104M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>AT25SF128A</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>AT25SL128A</td>
        <td>1.7-2.0V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>AT25QL128A</td>
        <td>1.7-2.0V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>ATXP032</td>
        <td>1.65-1.95V</td>
        <td>200M (x8)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td rowspan="4">Infineon<br>
                        (Spansion)</td>
        <td>S25FL064L</td>
        <td>2.7-3.6V</td>
        <td>108M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>S79FL256S</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>S25HL512T</td>
        <td>2.7-3.6V</td>
        <td>166M (x4)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td>S28HS512T</td>
        <td>1.7-2.0V</td>
        <td>200M (x8)</td>
        <td></td>
        <td>待定</td>
    </tr>
    <tr>
        <td rowspan="1">普冉<br>
                        PUYA</td>
        <td>PY25Q128HA</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td>S104Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td rowspan="1">东芯<br>
                        Dosilicon</td>
        <td>DS25M4AB</td>
        <td>1.65-1.95V</td>
        <td>104M (x4)</td>
        <td>S104Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td rowspan="1">武汉新芯<br>
                        XMC</td>
        <td>XM25QH64D</td>
        <td>2.7-3.6V</td>
        <td>166M (x4)</td>
        <td>S133Mx4</td>
        <td>通过</td>
    </tr>
    <tr>
        <td rowspan="1">博雅<br>
                        Boya</td>
        <td>BY25FQ64ES</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td>S104Mx4</td>
        <td>待定</td>
    </tr>
    <tr>
        <td rowspan="1">芝芯<br>
                        Zbit</td>
        <td>ZB25VQ64D</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td>S104Mx4</td>
        <td>待定</td>
    </tr>
    <tr>
        <td rowspan="1">芯天下<br>
                        XTX</td>
        <td>XT25F64F</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td>S104Mx4</td>
        <td>待定</td>
    </tr>
    <tr>
        <td rowspan="1">聚辰<br>
                        Giantec</td>
        <td>GT25Q64A</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td>S104Mx4</td>
        <td>待定</td>
    </tr>
    <tr>
        <td rowspan="1">复旦微<br>
                        FMSH</td>
        <td>FM25LQ64</td>
        <td>1.65-1.95V</td>
        <td>133M (x4)</td>
        <td>S104Mx4</td>
        <td>待定</td>
    </tr>
</table>


下列测试结果基于 RT1180-FLEXSPI-MEM-BB&DC_Rev.B（默认板卡设置）FlexSPI2 PortA MUX2 （GPIO_AON_XX），这组 Pin 也是 RT1186-FRDM 默认选择。

```text
FlexSPI1 pin multiplex options performance specifications:
1. GPIO_B1_XX and GPIO_B2_XX support the maximum 200 MHz operation frequency;
2. GPIO_SD_B2_XX only supports the maximum 166 MHz operation frequency.

FlexSPI2 pin multiplex options performance specifications:
1. GPIO_EMC_B1_XX supports the maximum 166 MHz operation frequency;
2. GPIO_AON_XX only supports the maximum 104 MHz operation frequency
```

```text
- 确保BB-[J34-45]-1&2, J122-Open, Remove R61/R1073, FLEXSPI2_MUX_SEL=1(上拉)
- 测试仅改动 BB-J62&J61 以设置 Flash_VCC: 3.3V/1.8V/1.2V
  -- J62.1-2 3.3V
  -- J62.2-3 1.8V
  -- J61.1-2 1.2V
- 测试仅改动 BB-J2 以切换 QUAD/OCTAL Package
  -- J2.1-2 OCTAL
  -- J2.2-3 QUAD
```

<table><tbody>
    <tr>
        <th>厂商</th>
        <th>型号</th>
        <th>电压</th>
        <th>速度</th>
        <th>测试项目</th>
        <th>测试结果</th>
    </tr>
    <tr>
        <td rowspan="1">芯成 ISSI</td>
        <td>IS25LP01G</td>
        <td>2.7-3.6V</td>
        <td>133M (x4)</td>
        <td>S104Mx4</td>
        <td>通过</td>
    </tr>
</table>


