# imxrt-multi-flash-boot-solution
一种灵活的i.MXRT下多串行NOR Flash型号选择的量产方案

![](/implementation_note.PNG)

### 已测试型号表

测试结果基于 RT1180-FLEXSPI-MEM-BB&DC_Rev.B（默认板卡设置）FlexSPI1 PortA MUX1

- 测试仅改动 BB-J68&J16 以设置 Flash_VCC: 3.3V/1.8V/1.2V
- 测试仅改动 BB-J11 以切换 QUAD/OCTAL

<table><tbody>
    <tr>
        <th>厂商</th>
        <th>型号</th>
        <th>电压</th>
        <th>类型</th>
        <th>备注</th>
    </tr>
    <tr>
        <td rowspan="9">华邦 Winbond</td>
        <td>W25Q40CLNIG</td>
        <td>2.3 - 3.6V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td>TBD，封装不便测试</td>
    </tr>
    <tr>
        <td>W25Q16JVSIQ</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 120MHz SDR<br>
            QuadSPI 133MHz SDR</td>
        <td>通过<br>
            回读校验失败</td>
    </tr>
    <tr>
        <td>W25Q32JVSIQ</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q64NESIG</td>
        <td>1.14V - 1.26V</td>
        <td>QuadSPI 84MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q64JWSJQ</td>
        <td>1.7 - 1.95V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q128JWSQ</td>
        <td>1.7 - 1.95V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q256JWBAQ</td>
        <td>1.7 - 1.95V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过 `</td>
    </tr>
    <tr>
        <td>W25M512JWBIQ</td>
        <td>1.7 - 1.95V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W35T51NW</td>
        <td>1.7 - 2.0V</td>
        <td>OctalSPI 100MHz DDR<br>
            OctalSPI 166MHz DDR<br>
            OctalSPI 200MHz DDR</td>
        <td>通过<br>
            回读校验失败<br>
            回读校验失败</td>
    </tr>
    <tr>
        <td rowspan="21">旺宏 Macronix</td>
        <td>MX25V4035F</td>
        <td>2.3 - 3.6V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td>TBD，封装不便测试</td>
    </tr>
    <tr>
        <td>MX25V8035F</td>
        <td>2.3 - 3.6V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25V1635F</td>
        <td>2.3 - 3.6V</td>
        <td>QuadSPI 80MHz SDR</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下四线初始化挂起</td>
    </tr>
    <tr>
        <td>MX25U1632F</td>
        <td>1.65 - 2V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下QE设置失败</td>
    </tr>
    <tr>
        <td>MX25L3233F</td>
        <td>2.65 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25U3232F</td>
        <td>1.65 - 2V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下QE设置失败</td>
    </tr>
    <tr>
        <td>MX25U6435F</td>
        <td>1.65 - 2V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td>QE设置失败</td>
    </tr>
    <tr>
        <td>MX25U6432F</td>
        <td>1.65 - 2V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下QE设置失败</td>
    </tr>
    <tr>
        <td>MX25L6433F</td>
        <td>2.65 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25U12832F</td>
        <td>1.65 - 2V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下QE设置失败</td>
    </tr>
    <tr>
        <td>MX25L12833F</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>JEDEC ID获取失败<br>
            假JEDEC ID下QE设置失败</td>
    </tr>
    <tr>
        <td>MX25L25645G</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>QE设置失败</td>
    </tr>
    <tr>
        <td>MX25U25643G</td>
        <td>1.65 - 2V</td>
        <td>QuadSPI 120MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MX25U25643G</td>
        <td>1.65 - 2V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25UW6435G</td>
        <td>1.65 - 2V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MX25UW12845G</td>
        <td>1.65 - 2V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MX25UW25345G</td>
        <td>1.65 - 2V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25UW51345G</td>
        <td>1.65 - 2V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MX25UM51345G</td>
        <td>1.65 - 2V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25LM51245G</td>
        <td>2.7 - 3.6V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MX66UM1G45G</td>
        <td>1.65 - 2V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td></td>
    </tr>
    <tr>
        <td rowspan="18">兆易创新 GigaDevice</td>
        <td>GD25LE32</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>GD25Q64CSIG</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 120MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD25LQ64</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>GD25LE64</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>GD25LB64ESIG</td>
        <td>1.65 - 2.0V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>回读校验失败</td>
    </tr>
    <tr>
        <td>GD25Q127CSIG</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD25B128ESIG</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD25LE128DSIG</td>
        <td>1.65 - 2.0V</td>
        <td>QuadSPI 120MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD25LQ128</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>GD25LF128</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>GD25LT256EB1R</td>
        <td>1.65 - 2.0V</td>
        <td>QuadSPI 166MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD25LB256</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>GD25LQ256</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>GD25LF255</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>GD25LT512</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>GD55B01G</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>GD25X512MEB2R</td>
        <td>2.7 - 3.6V</td>
        <td>OctalSPI 166MHz DDR<br>
            OctalSPI 200MHz DDR</td>
        <td>通过<br>
            回读校验失败</td>
    </tr>
    <tr>
        <td>GD25LX512MEB1R</td>
        <td>1.65 - 2.0V</td>
        <td>OctalSPI 166MHz DDR<br>
            OctalSPI 200MHz DDR</td>
        <td>通过<br>
            回读校验失败</td>
    </tr>
    <tr>
        <td rowspan="5">芯成 ISSI</td>
        <td>IS25LP064A</td>
        <td>2.3 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>IS25WP064A</td>
        <td>1.65 - 1.95V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>IS25LP064D</td>
        <td>2.3 - 3.6V</td>
        <td>QuadSPI 166MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>IS25WP064D</td>
        <td>1.65 - 1.95V</td>
        <td>QuadSPI 166MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>IS25WX256</td>
        <td>1.7 - 2.0V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td></td>
    </tr>
    <tr>
        <td rowspan="5">镁光 Micron</td>
        <td>MT25QU128_RW138</td>
        <td>1.7 - 2.0V</td>
        <td>QuadSPI 166MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MT25QL128_RW126</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MT25QL256_RW162</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MT35XU512_RW303</td>
        <td>1.7 - 2.0V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MT35XU512_RW304</td>
        <td>1.7 - 2.0V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td></td>
    </tr>
    <tr>
        <td rowspan="6">Renesas (Adesto)</td>
        <td>AT25QF641</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>AT25QL641</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>AT25SF128A</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>AT25SL128A</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>AT25QL128A</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>ATXP032</td>
        <td></td>
        <td>OctalSPI 200MHz DDR</td>
        <td></td>
    </tr>
    <tr>
        <td rowspan="2">Infineon (Spansion)</td>
        <td>S25FL064L</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>S28HS512</td>
        <td></td>
        <td>OctalSPI 200MHz DDR</td>
        <td></td>
    </tr>
</table>
